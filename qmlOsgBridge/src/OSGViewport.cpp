#include "qmlOsgBridge/OSGViewport.h"

#include <qmlOsgBridge/IRenderer.h>

#include "DrawCallbackFunc.h"
#include "Window.h"

namespace qmlOsgBridge
{

OSGViewport::OSGViewport(QQuickItem* parent) :
  QQuickItem(parent),
  m_window(nullptr),
  m_textureNode(nullptr),
  m_context(new osgViewer::GraphicsWindowEmbedded(0, 0, 1, 1)),
  m_remainingSizeUpdateSteps(2),
  m_needPrepareNodesUpdate(false)
{
  m_format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);

  forceActiveFocus();
  setAcceptHoverEvents(true);
}

OSGViewport::~OSGViewport()
{
  if (m_window)
  {
    m_window->removeViewport(*this);
  }
}

void OSGViewport::classBegin()
{
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::AllButtons);

  QQuickItem::classBegin();
}

void OSGViewport::acceptWindow(IWindow* window)
{
  m_window = window;

  updateViewport();
  setFlag(ItemHasContents, true);
}

void OSGViewport::prepareNode()
{
  if (!m_displayFbo && !m_needPrepareNodesUpdate)
  {
    return;
  }

  if(m_needPrepareNodesUpdate)
  {
    if (m_displayTexture)
    {
      delete m_displayTexture.data();
    }

    const auto textureHandle = m_displayFbo->texture();
    m_displayTexture = m_window->getQuickWindow()->createTextureFromNativeObject(
      QQuickWindow::NativeObjectTexture, &textureHandle, 0, m_renderSize);

    if(!m_textureNode)
    {
      m_textureNode = new QSGSimpleTextureNode();
      QMetaObject::invokeMethod(this, &QQuickItem::update, Qt::QueuedConnection);
    }
    m_textureNode->setRect(0, height(), width(), -height());

    m_needPrepareNodesUpdate = false;
  }

  m_textureNode->setTexture(m_displayTexture);
}

void OSGViewport::deleteFrameBufferObjects()
{
  m_renderFbo.reset();
  m_displayFbo.reset();
}

void OSGViewport::installEventFilter(QObject* eventFilter)
{
  if (std::find(m_eventFilters.begin(), m_eventFilters.end(), eventFilter) != m_eventFilters.end())
  {
    return;
  }

  QQuickItem::installEventFilter(eventFilter);
  m_eventFilters.emplace_back(eventFilter);
}

void OSGViewport::removeEventFilter(QObject* eventFilter)
{
  QQuickItem::removeEventFilter(eventFilter);
  static_cast<void>(std::remove(m_eventFilters.begin(), m_eventFilters.end(), eventFilter));
}

osg::ref_ptr<osgViewer::View> OSGViewport::getView() const
{
  return m_renderer->getView();
}

QPointer<IRenderer> OSGViewport::renderer() const
{
  return m_renderer;
}

void OSGViewport::setRenderer(const QPointer<IRenderer>& renderer)
{
  if (m_renderer)
  {
    UTILS_LOG_WARN("Unable to reset renderer");
  }

  m_renderer = renderer;
  m_renderer->setContextWindow(m_window);
  m_renderer->setOSGViewport(this);

  const auto preDrawCallback = new DrawCallbackFunc(std::bind(&OSGViewport::preDrawFunction, this));
  const auto postDrawCallback = new DrawCallbackFunc(std::bind(&OSGViewport::postDrawFunction, this));

  auto view = m_renderer->getView();
  auto* ea = view->getEventQueue()->getCurrentEventState();
  ea->setMouseYOrientation(osgGA::GUIEventAdapter::Y_INCREASING_UPWARDS);

  view->getCamera(osgHelper::View::CameraType::Screen)->setPreDrawCallback(preDrawCallback);
  view->getCamera(osgHelper::View::CameraType::Screen)->setPostDrawCallback(postDrawCallback);
  view->getCamera(osgHelper::View::CameraType::Screen)->setGraphicsContext(m_context);
  view->getCamera(osgHelper::View::CameraType::Scene)->setGraphicsContext(m_context);
  view->getEventQueue()->setGraphicsContext(m_context);

  m_window->addViewport(*this);
}

QSGNode* OSGViewport::updatePaintNode(
  QSGNode* oldNode,
  QQuickItem::UpdatePaintNodeData* updatePaintNodeData)
{
  if (oldNode && oldNode != m_textureNode)
  {
    qDebug() << "[osgQtQuick] ViewQtQuick::updatePaintNode delete old node " << oldNode;
    delete oldNode;
  }

  if (m_window->initializeRenderContextIfNecessary())
  {
    return nullptr;
  }

  return m_textureNode;
}

void OSGViewport::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
  const auto size = boundingRect().size().toSize();

  updateViewport();

  if(!m_textureNode)
  {
    m_renderSize = size;
  }

  QQuickItem::geometryChanged(newGeometry, oldGeometry);
}

void OSGViewport::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData& value)
{
  if(change == QQuickItem::ItemSceneChange && value.window)
  {
    acceptWindow(Window::fromQuickWindow(value.window));
  }
  QQuickItem::itemChange(change, value);
}

void OSGViewport::mousePressEvent(QMouseEvent* event)
{
}

void OSGViewport::hoverMoveEvent(QHoverEvent* event)
{
  QMouseEvent mouseEvent(QEvent::MouseMove, event->pos(),
    Qt::MouseButton::NoButton, Qt::MouseButton::NoButton, Qt::KeyboardModifier::NoModifier);

  for (const auto& filter : m_eventFilters)
  {
    filter->eventFilter(this, &mouseEvent);
  }
}

void OSGViewport::preDrawFunction()
{
  if(m_remainingSizeUpdateSteps > 0)
  {
    m_renderFbo = std::make_unique<QOpenGLFramebufferObject>(m_renderSize, m_format);

    --m_remainingSizeUpdateSteps;
    m_needPrepareNodesUpdate = true;
  }
  m_renderFbo->bind();
}

void OSGViewport::postDrawFunction()
{
  if (!m_renderFbo || !m_window)
  {
    return;
  }

  m_window->flush();
  m_renderFbo->bindDefault();

  std::swap(m_renderFbo, m_displayFbo);
  std::swap(m_renderTexture, m_displayTexture);
}

void OSGViewport::updateViewport()
{
  const auto size = boundingRect().size().toSize();
  if (m_textureNode)
  {
    if (m_renderSize != size)
    {
      m_window->dispatchRenderThreadBlocking([this, size]()
      {
        m_renderer->getView()->updateResolution(osg::Vec2i(size.width(), size.height()));

        m_renderSize = size;
        m_remainingSizeUpdateSteps = m_remainingSizeUpdateSteps % 2 + 2;
      });
    }
  }
  else if (m_renderer)
  {
    m_renderer->getView()->updateResolution(osg::Vec2i(size.width(), size.height()));
  }
}

}
