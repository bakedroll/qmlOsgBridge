#pragma once

#include <QObject>
#include <QQuickItem>

#include <functional>

#include <osg/ref_ptr>

namespace osgHelper
{
class View;
}

namespace qmlOsgBridge
{

class IQmlGameProxy : public QObject
{
  Q_OBJECT

public:
  IQmlGameProxy() = default;
  ~IQmlGameProxy() override = default;

  virtual osg::ref_ptr<osgHelper::View> getView() const = 0;
  virtual QPointer<QQuickItem> getViewportQuickItem() const = 0;

  virtual void setViewportQuickItem(const QPointer<QQuickItem>& item) = 0;

  virtual void executeMutexLocked(const std::function<void()>& func) = 0;

};

}
