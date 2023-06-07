#include <qmlOsgBridge/OSGViewport.h>

#include <QSGSimpleTextureNode>
#include <QTimer>

#include <osgGA/EventQueue>

#include "RendererImpl.h"
#include "WindowsStorage.h"

namespace qmlOsgBridge
{

static const int s_FrameIntervalMs = 16;

static int qtToOsgMouseButton(const QMouseEvent& event)
{
  switch (event.button())
  {
    case Qt::LeftButton:
      return 1;
    case Qt::MiddleButton:
      return 2;
    case Qt::RightButton:
      return 3;
    default:
      break;
  }
  return 0;
}

OSGViewport::OSGViewport(QQuickItem* parent) :
  QQuickFramebufferObject(parent),
  m_viewer(new osgViewer::CompositeViewer()),
  m_window(new osgViewer::GraphicsWindowEmbedded(0, 0, 1, 1)),
  m_pendingEvents(new osgGA::EventQueue(osgGA::GUIEventAdapter::Y_INCREASING_DOWNWARDS))
{
  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
  forceActiveFocus();

  m_viewer->setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
  m_viewer->setKeyEventSetsDone(0);
  m_viewer->setReleaseContextAtEndOfFrameHint(false);

  connect(&m_frameTimer, &QTimer::timeout, this, &QQuickFramebufferObject::update);

  m_frameTimer.setInterval(s_FrameIntervalMs);
  m_frameTimer.setSingleShot(false);

  m_frameTimer.start();
}

OSGViewport::~OSGViewport() = default;

QPointer<IQmlGameProxy> OSGViewport::proxy() const
{
  return m_proxy;
}

void OSGViewport::setProxy(const QPointer<IQmlGameProxy>& proxy)
{
  m_proxy = proxy;

  m_proxy->setViewportQuickItem(this);

  auto view = m_proxy->getView();
  view->getCamera(osgHelper::View::CameraType::Screen)->setGraphicsContext(m_window.get());
  view->getCamera(osgHelper::View::CameraType::Scene)->setGraphicsContext(m_window.get());
  view->getEventQueue()->setGraphicsContext(m_window.get());

  m_viewer->addView(view);
}

osg::ref_ptr<osgViewer::CompositeViewer> OSGViewport::getViewer() const
{
  return m_viewer;
}

osg::ref_ptr<osgViewer::GraphicsWindow> OSGViewport::getGraphicsWindow() const
{
  return m_window;
}

QSize OSGViewport::getSize() const
{
  return QSize(static_cast<int>(width()), static_cast<int>(height()));
}

osg::ref_ptr<osgGA::EventQueue> OSGViewport::getPendingEvents() const
{
  return m_pendingEvents;
}

QQuickFramebufferObject::Renderer* OSGViewport::createRenderer() const
{
  return new RendererImpl(*this, m_proxy);
}

QSGNode* OSGViewport::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* upnData)
{
  const auto node = QQuickFramebufferObject::updatePaintNode(oldNode, upnData);
  auto textureNode = dynamic_cast<QSGSimpleTextureNode*>(node);
  if (textureNode)
  {
    textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
  }
  return node;
}

void OSGViewport::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
  m_pendingEvents->windowResize(newGeometry.x(), newGeometry.y(), newGeometry.width(), newGeometry.height());
  QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void OSGViewport::itemChange(ItemChange change, const ItemChangeData& value)
{
  if (change == ItemSceneChange && value.window)
  {
    WindowsStorage::get().addQuickWindow(value.window);
  }
  QQuickItem::itemChange(change, value);
}

void OSGViewport::mousePressEvent(QMouseEvent* event)
{
  // intentionally not calling base implementation
  m_pendingEvents->mouseButtonPress(
    static_cast<float>(event->x()),
    static_cast<float>(event->y()), 
    qtToOsgMouseButton(*event));

  forceActiveFocus();
}

void OSGViewport::mouseMoveEvent(QMouseEvent* event)
{
  QQuickFramebufferObject::mouseMoveEvent(event);
  m_pendingEvents->mouseMotion(static_cast<float>(event->x()), static_cast<float>(event->y()));
}

void OSGViewport::mouseReleaseEvent(QMouseEvent* event)
{
  QQuickFramebufferObject::mouseReleaseEvent(event);
  m_pendingEvents->mouseButtonRelease(
    static_cast<float>(event->x()),
    static_cast<float>(event->y()),
    qtToOsgMouseButton(*event));
}

void OSGViewport::mouseDoubleClickEvent(QMouseEvent* event)
{
  QQuickFramebufferObject::mouseDoubleClickEvent(event);
  m_pendingEvents->mouseDoubleButtonPress(
    static_cast<float>(event->x()),
    static_cast<float>(event->y()),
    qtToOsgMouseButton(*event));
}

void OSGViewport::wheelEvent(QWheelEvent* event)
{
  QQuickFramebufferObject::wheelEvent(event);
  if (event->angleDelta().y() > 0)
  {
    m_pendingEvents->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
  }
  else
  {
    m_pendingEvents->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
  }
}

void OSGViewport::keyPressEvent(QKeyEvent* event)
{
  QQuickFramebufferObject::keyPressEvent(event);
  const std::string text = event->text().toStdString();
  if (text.size() == 1)
  {
    m_pendingEvents->keyPress(text[0]);
  }
}

}
