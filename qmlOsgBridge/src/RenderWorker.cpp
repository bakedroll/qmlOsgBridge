#include "RenderWorker.h"

#include <qmlOsgBridge/IOSGViewport.h>

#include "IWindow.h"

#include <QOpenGLFunctions>
#include <QThread>

namespace qmlOsgBridge
{

RenderWorker::RenderWorker(IWindow& window, QObject* parent) :
  QObject(parent),
  m_window(window),
  m_nextFrameTimer(new QTimer(this)),
  m_shuttingDown(false)
{
  m_nextFrameTimer->setSingleShot(true);
  m_frameTimeMeasure.start();

  connect(m_nextFrameTimer, &QTimer::timeout, this, &RenderWorker::requestNext);
}

RenderWorker::~RenderWorker() = default;

bool RenderWorker::hasContext() const
{
  return m_context;
}

void RenderWorker::setupContext(const QPointer<QQuickWindow>& window, const QPointer<QThread>& renderThread)
{
  const auto current = window->openglContext();
  current->doneCurrent();

  m_context = new QOpenGLContext();
  m_context->setFormat(current->format());
  m_context->setShareContext(current);
  m_context->create();
  m_context->moveToThread(renderThread);

  current->makeCurrent(window);
}

void RenderWorker::setupSurface()
{
  m_surface = new QOffscreenSurface();
  m_surface->setFormat(m_context->format());
  m_surface->create();
}

void RenderWorker::flush()
{
  m_context->functions()->glFlush();
}

void RenderWorker::render()
{
  if (m_shuttingDown)
  {
    return;
  }

  m_context->makeCurrent(m_surface);

  m_window.frame();

  const auto minFrameTimeNs = m_window.getMinFrameTimeMs();
  const auto frameTimeNs = m_frameTimeMeasure.elapsed();

  if (frameTimeNs < minFrameTimeNs)
  {
    m_nextFrameTimer->setInterval(minFrameTimeNs - frameTimeNs);
    m_nextFrameTimer->start();
    return;
  }

  requestNext();
}

void RenderWorker::shutdown()
{
  m_shuttingDown = true;

  m_context->makeCurrent(m_surface);
  m_window.deleteFbos();
  m_context->doneCurrent();

  m_context->deleteLater();
  m_surface->deleteLater();
}

void RenderWorker::requestNext()
{
  m_frameTimeMeasure.start();
  Q_EMIT textureReady();
}

void RenderWorker::dispatch(const std::function<void()>& func)
{
  func();
}

}
