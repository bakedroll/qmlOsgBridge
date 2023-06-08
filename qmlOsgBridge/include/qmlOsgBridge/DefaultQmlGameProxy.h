#pragma once

#include <QMutex>

#include <qmlOsgBridge/IQmlGameProxy.h>

namespace qmlOsgBridge
{

class DefaultQmlGameProxy : public IQmlGameProxy
{
  Q_OBJECT

public:
  DefaultQmlGameProxy();
  ~DefaultQmlGameProxy() override;

  osg::ref_ptr<osgHelper::View> getView() const override;
  QPointer<QQuickItem> getViewportQuickItem() const override;

  void setViewportQuickItem(const QPointer<QQuickItem>& item) override;

  void setRenderThread(const QPointer<QThread>& renderThread) override;
  QPointer<QThread> getRenderThread() const override;

  void executeMutexLocked(const std::function<void()>& func) override;

private:
  osg::ref_ptr<osgHelper::View> m_view;
  QPointer<QThread> m_renderThread;

  // TODO: remove
  QMutex m_mutex;

  QPointer<QQuickItem> m_quickItem;

};

}
