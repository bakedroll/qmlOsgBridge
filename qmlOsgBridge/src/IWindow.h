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

  virtual void flush() = 0;
  virtual void frame() = 0;
  virtual void deleteFbos() = 0;

  virtual void addViewport(IOSGViewport& viewport) = 0;
  virtual void removeViewport(IOSGViewport& viewport) = 0;

  virtual bool initializeRenderContextIfNecessary() = 0;
  virtual void dispatchRenderThread(const std::function<void()>& func) = 0;
  virtual void dispatchRenderThreadBlocking(const std::function<void()>& func) = 0;

  virtual QPointer<QThread> getRenderThread() const = 0;

};

}
