#include "Window.h"

#include <qmlOsgBridge/IOSGViewport.h>

#include <QOpenGLFunctions>
#include <QOpenGLContext>

namespace qmlOsgBridge
{

static constexpr int c_minFrameTimeNs = 16; // 16 ms per frame

Window::Window(QQuickWindow* quickWindow) :
  QObject(quickWindow),
  m_quickWindow(quickWindow),
  m_viewer(new osgViewer::CompositeViewer())
{
  // TODO:
  //connect(m_quickWindow, &QQuickWindow::sceneGraphAboutToStop, this, &Window::onSceneGraphAboutToStop);
  connect(m_quickWindow, &QQuickWindow::beforeRendering, this, &Window::prepareNodes, Qt::DirectConnection);
  connect(this, &Window::pendingNewTexture, m_quickWindow, &QQuickWindow::update, Qt::QueuedConnection);

  m_quickWindow->setClearBeforeRendering(false);

  m_viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  m_viewer->setKeyEventSetsDone(0);
}

Window::~Window()
{
  m_windowsStorage.erase(m_windowsStorage.find(m_quickWindow));
}

QPointer<QQuickWindow> Window::getQuickWindow() const
{
  return m_quickWindow;
}

int Window::getMinFrameTimeMs() const
{
  return c_minFrameTimeNs;
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

QPointer<QThread> Window::getRenderThread() const
{
  return m_renderThread;
}

Window* Window::fromQuickWindow(QQuickWindow* quickWindow)
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

void Window::prepareNodes()
{
  if (!m_renderThread)
  {
    m_renderThread = QThread::currentThread();
    Q_EMIT renderThreadChanged(m_renderThread);
  }

  QOpenGLContext::currentContext()->functions()->glUseProgram(0);
  m_viewer->frame();

  for (const auto& viewport : m_viewports)
  {
    viewport->prepareNode();
  }

  Q_EMIT pendingNewTexture();
}

std::map<QQuickWindow*, Window*> Window::m_windowsStorage;

}
