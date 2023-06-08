#include "qmlOsgBridge/DefaultQmlGameProxy.h"

namespace qmlOsgBridge
{

DefaultQmlGameProxy::DefaultQmlGameProxy() :
  IQmlGameProxy(),
  m_view(new osgHelper::View())
{
}

DefaultQmlGameProxy::~DefaultQmlGameProxy() = default;

osg::ref_ptr<osgHelper::View> DefaultQmlGameProxy::getView() const
{
  return m_view;
}

QPointer<QQuickItem> DefaultQmlGameProxy::getViewportQuickItem() const
{
  return m_quickItem;
}

void DefaultQmlGameProxy::setViewportQuickItem(const QPointer<QQuickItem>& item)
{
  m_quickItem = item;
}

void DefaultQmlGameProxy::setRenderThread(const QPointer<QThread>& renderThread)
{
  m_renderThread = renderThread;
  Q_EMIT renderThreadPropagated(m_renderThread);
}

QPointer<QThread> DefaultQmlGameProxy::getRenderThread() const
{
  return m_renderThread;
}

void DefaultQmlGameProxy::executeMutexLocked(const std::function<void()>& func)
{
  QMutexLocker locker(&m_mutex);
  func();
}

}
