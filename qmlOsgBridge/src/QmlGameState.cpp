#include <qmlOsgBridge/QmlGameState.h>

namespace qmlOsgBridge
{

QmlGameState::QmlGameState(osgHelper::ioc::Injector& injector) :
  AbstractGameState(injector),
  m_isInitialized(false)
{
}

QmlGameState::~QmlGameState() = default;

void QmlGameState::onInitialize(const SimulationData& data)
{
  AbstractGameState::onInitialize(data);
}

void QmlGameState::onInitialize(const QPointer<IQmlGameProxy>& proxy, const SimulationData& data)
{
}

bool QmlGameState::isEventHandlingEnabled() const
{
  return m_eventHandler != nullptr;
}

libQtGame::KeyboardMouseEventFilter& QmlGameState::eventHandler() const
{
  return *m_eventHandler;
}

void QmlGameState::setInitialized()
{
  m_isInitialized = true;
}

bool QmlGameState::isInitialized() const
{
  return m_isInitialized;
}

bool QmlGameState::onKeyEvent(QKeyEvent* event)
{
  return false;
}

bool QmlGameState::onMouseEvent(QMouseEvent* event)
{
  return false;
}

bool QmlGameState::onWheelEvent(QWheelEvent* event)
{
  return false;
}

void QmlGameState::onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
}

void QmlGameState::onDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
}

void QmlGameState::onDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
}

void QmlGameState::enableEventHandling()
{
  if (m_eventHandler)
  {
    return;
  }

  m_eventHandler = std::make_unique<libQtGame::KeyboardMouseEventFilter>();

  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerKeyEvent,
    this, &QmlGameState::forwardKeyEvent, Qt::DirectConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerMouseEvent,
    this, &QmlGameState::forwardMouseEvent, Qt::DirectConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerWheelEvent,
    this, &QmlGameState::forwardWheelEvent, Qt::DirectConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragBegin,
    this, &QmlGameState::forwardDragBegin, Qt::DirectConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragMove,
    this, &QmlGameState::forwardDragMove, Qt::DirectConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragEnd,
    this, &QmlGameState::forwardDragEnd, Qt::DirectConnection);
}

void QmlGameState::forwardKeyEvent(QKeyEvent* event, bool& accepted)
{
  if (!m_isInitialized)
  {
    return;
  }

  accepted = onKeyEvent(event);
}

void QmlGameState::forwardMouseEvent(QMouseEvent* event, bool& accepted)
{
  if (!m_isInitialized)
  {
    return;
  }

  accepted = onMouseEvent(event);
}

void QmlGameState::forwardWheelEvent(QWheelEvent* event, bool& accepted)
{
  if (!m_isInitialized)
  {
    return;
  }

  accepted = onWheelEvent(event);
}

void QmlGameState::forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
  if (!m_isInitialized)
  {
    return;
  }

  onDragBegin(button, origin);
}

void QmlGameState::forwardDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
  if (!m_isInitialized)
  {
    return;
  }

  onDragMove(button, origin, position, change);
}

void QmlGameState::forwardDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
  if (!m_isInitialized)
  {
    return;
  }

  onDragEnd(button, origin, position);
}

}
