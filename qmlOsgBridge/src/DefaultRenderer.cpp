#include <qmlOsgBridge/DefaultRenderer.h>

#include "IWindow.h"

namespace qmlOsgBridge
{

DefaultRenderer::DefaultRenderer() :
  m_view(new osgHelper::View()),
  m_window(nullptr)
{
}

DefaultRenderer::~DefaultRenderer() = default;

osg::ref_ptr<osgHelper::View> DefaultRenderer::getView() const
{
  return m_view;
}

QPointer<QQuickWindow> DefaultRenderer::getQuickWindow() const
{
  if (!m_window)
  {
    return {};
  }
  return m_window->getQuickWindow();
}

QPointer<QQuickItem> DefaultRenderer::getViewportItem() const
{
  return m_viewport;
}

bool DefaultRenderer::hasRenderThread() const
{
  return m_window->getRenderThread() != nullptr;
}

void DefaultRenderer::dispatchRenderThread(const std::function<void()>& func) const
{
  if (!m_window || !m_window->getRenderThread())
  {
    UTILS_LOG_ERROR("Could not dispatch to render thread");
    return;
  }
  // TODO:
  //m_window->dispatchRenderThread(func);
}

void DefaultRenderer::moveObjectToRenderThread(const QPointer<QObject>& obj) const
{
  obj->moveToThread(m_window->getRenderThread());
}

void DefaultRenderer::setContextWindow(IWindow* window)
{
  m_window = window;
}

void DefaultRenderer::setViewportItem(const QPointer<QQuickItem>& viewport)
{
  m_viewport = viewport;
}

}
