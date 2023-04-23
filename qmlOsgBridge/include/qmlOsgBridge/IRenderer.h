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
  virtual QPointer<QQuickItem> getViewportItem() const = 0;

  virtual bool hasRenderThread() const = 0;
  virtual void dispatchRenderThread(const std::function<void()>& func) const = 0;
  virtual void moveObjectToRenderThread(const QPointer<QObject>& obj) const = 0;

  virtual void setContextWindow(IWindow* window) = 0;
  virtual void setViewportItem(const QPointer<QQuickItem>& viewport) = 0;

Q_SIGNALS:
  void renderThreadChanged(QThread* renderThread);

};

}
