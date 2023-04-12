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

void DefaultRenderer::dispatchRenderThread(const std::function<void()>& func) const
{
  if (!m_window)
  {
    UTILS_LOG_ERROR("Could not dispatch to render thread due to unknown context window");
    return;
  }
  m_window->dispatchRenderThread(func);
}

void DefaultRenderer::moveObjectToRenderThread(const QPointer<QObject>& obj) const
{
  obj->moveToThread(m_window->getRenderThread());
}

void DefaultRenderer::setContextWindow(IWindow* window)
{
  m_window = window;
}

}
