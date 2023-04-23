#pragma once

#include <QPointer>
#include <QQuickWindow>
#include <QThread>

#include <functional>

namespace qmlOsgBridge
{

class IOSGViewport;

class IWindow
{
public:
  virtual ~IWindow() = default;

  virtual QPointer<QQuickWindow> getQuickWindow() const = 0;

  virtual void frame() = 0;
  virtual int getMinFrameTimeMs() const = 0;

  virtual void addViewport(IOSGViewport& viewport) = 0;
  virtual void removeViewport(IOSGViewport& viewport) = 0;

  virtual QPointer<QThread> getRenderThread() const = 0;

};

}
