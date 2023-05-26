#pragma once

#include <QQuickFramebufferObject>

#include <qmlOsgBridge/IQmlGameProxy.h>

namespace qmlOsgBridge
{

class IOSGViewport;

class RendererImpl : public QQuickFramebufferObject::Renderer
{
public:
  RendererImpl(const IOSGViewport& viewport, const QPointer<IQmlGameProxy>& proxy);
  ~RendererImpl() override;

  void synchronize(QQuickFramebufferObject* item) override;
  void render() override;
  QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) override;

private:
  const IOSGViewport& m_viewport;
  QPointer<IQmlGameProxy> m_proxy;

  bool m_isDefaultFboSet;

  bool sizeHasChanged() const;
  void updateSize();

};

}
