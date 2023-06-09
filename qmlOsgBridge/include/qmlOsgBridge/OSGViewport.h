#pragma once

#include <QQuickFramebufferObject>

#include <QTimer>

#include <qmlOsgBridge/IOSGViewport.h>
#include <qmlOsgBridge/IQmlGameProxy.h>

namespace qmlOsgBridge
{

class OSGViewport : public QQuickFramebufferObject,
                    public IOSGViewport
{
  Q_OBJECT

  Q_PROPERTY(QPointer<IQmlGameProxy> proxy READ proxy WRITE setProxy NOTIFY changedProxy)

public:
  OSGViewport(QQuickItem* parent = nullptr);
  ~OSGViewport() override;

  QPointer<IQmlGameProxy> proxy() const;
  void setProxy(const QPointer<IQmlGameProxy>& proxy);

  osg::ref_ptr<osgViewer::CompositeViewer> getViewer() const override;
  osg::ref_ptr<osgViewer::GraphicsWindow> getGraphicsWindow() const override;
  QPointer<QQuickWindow> getQuickWindow() const override;

  QSize getSize() const override;
  osg::ref_ptr<osgGA::EventQueue> getPendingEvents() const override;

  Renderer* createRenderer() const override;

Q_SIGNALS:
  void changedProxy();

protected:
  QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* upnData) override;
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
  void itemChange(ItemChange change, const ItemChangeData& value) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseDoubleClickEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent*) override;
  void keyPressEvent(QKeyEvent*) override;

private:
  osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;
  osg::ref_ptr<osgViewer::GraphicsWindow> m_window;
  QPointer<QQuickWindow> m_quickWindow;

  QPointer<IQmlGameProxy> m_proxy;
  osg::ref_ptr<osgGA::EventQueue> m_pendingEvents;

  QTimer m_frameTimer;

};

}
