#pragma once

#include <QObject>
#include <QQuickWindow>

#include <osgHelper/View.h>

#include <functional>

namespace qmlOsgBridge
{

class IWindow;

class IRenderer : public QObject
{
  Q_OBJECT

public:
  IRenderer() = default;
  ~IRenderer() override = default;

  virtual osg::ref_ptr<osgHelper::View> getView() const = 0;
  virtual QPointer<QQuickWindow> getQuickWindow() const = 0;
  virtual void dispatchRenderThread(const std::function<void()>& func) const = 0;
  virtual void moveObjectToRenderThread(const QPointer<QObject>& obj) const = 0;

  virtual void setContextWindow(IWindow* window) = 0;


};

}
