#include "Window.h"

#include <qmlOsgBridge/IOSGViewport.h>

#include <QOpenGLFunctions>
#include <QOpenGLContext>

#include "RenderWorker.h"

namespace qmlOsgBridge
{

static constexpr int c_minFrameTimeNs = 16; // 16 ms per frame

Window::Window(QQuickWindow* quickWindow) :
  QObject(quickWindow),
  m_quickWindow(quickWindow),
  m_viewer(new osgViewer::CompositeViewer()),
  m_renderThread(new QThread(this)),
  m_renderWorker(std::make_unique<RenderWorker>(*this)),
  m_isNewTexture(false),
  m_isReady(false),
  m_frameTimer(-1)
{
  /*connect(this, &Window::triggerDispatchRenderThread, m_renderWorker.get(), &RenderWorker::dispatch, Qt::QueuedConnection);
  connect(this, &Window::triggerDispatchRenderThreadBlocking, m_renderWorker.get(), &RenderWorker::dispatch, Qt::BlockingQueuedConnection);
  connect(this, &Window::textureInUse, m_renderWorker.get(), &RenderWorker::render, Qt::QueuedConnection);

  connect(m_renderWorker.get(), &RenderWorker::textureReady, this, &Window::newTexture, Qt::QueuedConnection);
  connect(m_renderWorker.get(), &RenderWorker::textureReady, m_quickWindow, &QQuickWindow::update, Qt::QueuedConnection);

  connect(m_quickWindow, &QQuickWindow::beforeRendering, this, &Window::prepareNodes, Qt::DirectConnection);*/

  connect(m_quickWindow, &QQuickWindow::sceneGraphAboutToStop, this, &Window::onSceneGraphAboutToStop);
  connect(m_quickWindow, &QQuickWindow::beforeRendering, this, &Window::prepareNodes, Qt::DirectConnection);
  connect(this, &Window::pendingNewTexture, m_quickWindow, &QQuickWindow::update, Qt::QueuedConnection);

  m_quickWindow->setClearBeforeRendering(false);

  m_viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  m_viewer->setKeyEventSetsDone(0);
}

Window::~Window()
{
  if (m_renderThread->isRunning())
  {
    m_renderThread->quit();
    m_renderThread->wait();
  }

  m_windowsStorage.erase(m_windowsStorage.find(m_quickWindow));
}

QPointer<QQuickWindow> Window::getQuickWindow() const
{
  return m_quickWindow;
}

void Window::flush()
{
  m_renderWorker->flush();
}

void Window::frame()
{
  /*m_viewer->frame();*/

  // TODO:
  { // Qt bug!?
    QOpenGLContext::currentContext()->functions()->glUseProgram(0);
  }
  m_viewer->frame();
  m_isNewTexture = true;
  prepareNodes();
}

void Window::deleteFbos()
{
  for (const auto& viewport : m_viewports)
  {
    viewport->deleteFrameBufferObjects();
  }
}

int Window::getMinFrameTimeMs() const
{
  return c_minFrameTimeNs;
}

bool Window::isReady() const
{
  return m_isReady;
}

void Window::setReady()
{
  QMetaObject::invokeMethod(this, &Window::ready);
}

void Window::addViewport(IOSGViewport& viewport)
{
  m_viewports.insert(&viewport);
  m_viewer->addView(viewport.getView());
}

void Window::removeViewport(IOSGViewport& viewport)
{
  m_viewports.erase(&viewport);
  m_viewer->removeView(viewport.getView());
}

bool Window::initializeRenderContextIfNecessary()
{
  if (m_renderWorker->hasContext())
  {
    return false;
  }

  m_renderWorker->setupContext(m_quickWindow, m_renderThread);
  QMetaObject::invokeMethod(this, &Window::ready);
  return true;
}

void Window::dispatchRenderThread(const std::function<void()>& func)
{
  Q_EMIT triggerDispatchRenderThread(func);
}

void Window::dispatchRenderThreadBlocking(const std::function<void()>& func)
{
  Q_EMIT triggerDispatchRenderThreadBlocking(func);
}

QPointer<QThread> Window::getRenderThread() const
{
  return m_renderThread;
}

IWindow* Window::fromQuickWindow(QQuickWindow* quickWindow)
{
  if (m_windowsStorage.count(quickWindow) == 0)
  {
    m_windowsStorage[quickWindow] = new Window(quickWindow);
  }
  return m_windowsStorage.at(quickWindow);
}

void Window::closeAll()
{
  for (const auto& window : m_windowsStorage)
  {
    window.first->close();
  }
}

void Window::ready()
{
  /*m_renderWorker->setupSurface();
  m_renderWorker->moveToThread(m_renderThread);

  connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, m_renderWorker.get(), &RenderWorker::shutdown, Qt::QueuedConnection);
  connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, m_renderThread, &QThread::quit, Qt::QueuedConnection);

  m_renderThread->start();

  QMetaObject::invokeMethod(m_renderWorker.get(), &RenderWorker::render, Qt::QueuedConnection);*/

  m_isReady = true;
  m_frameTimer = startTimer(getMinFrameTimeMs(), Qt::TimerType::PreciseTimer);
}

void Window::newTexture()
{
  m_isNewTexture = true;
}

void Window::prepareNodes()
{
  if(m_isNewTexture)
  {
    for (const auto& viewport : m_viewports)
    {
      viewport->prepareNode();
    }

    m_isNewTexture = false;
    Q_EMIT textureInUse();
  }
  Q_EMIT pendingNewTexture();
}

void Window::onSceneGraphAboutToStop()
{
  if(m_frameTimer != -1)
  {
    killTimer(m_frameTimer);
    m_frameTimer = -1;
  }
}

void Window::timerEvent(QTimerEvent* event)
{
  if (event->timerId() == m_frameTimer)
  {
    frame();
  }

  QObject::timerEvent(event);
}

std::map<QQuickWindow*, IWindow*> Window::m_windowsStorage;

}
