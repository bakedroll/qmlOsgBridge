#pragma once

#include <qmlOsgBridge/IRenderer.h>

#include <osgHelper/View.h>

namespace qmlOsgBridge
{

class IOSGViewport;

class DefaultRenderer : public IRenderer
{
  Q_OBJECT

public:
  DefaultRenderer();
  ~DefaultRenderer() override;

  osg::ref_ptr<osgHelper::View> getView() const override;
  QPointer<QQuickWindow> getQuickWindow() const override;
  IOSGViewport* getOSGViewport() const override;

  void dispatchRenderThread(const std::function<void()>& func) const override;
  void moveObjectToRenderThread(const QPointer<QObject>& obj) const override;

  void setContextWindow(IWindow* window) override;
  void setOSGViewport(IOSGViewport* viewport) override;

private:
  osg::ref_ptr<osgHelper::View> m_view;
  IWindow* m_window;
  IOSGViewport* m_viewport;

};

}
