#include <qmlOsgBridge/QmlGameState.h>

namespace qmlOsgBridge
{

QmlGameState::QmlGameState(osgHelper::ioc::Injector& injector) :
  AbstractGameState(injector)
{
}

QmlGameState::~QmlGameState() = default;

void QmlGameState::onInitialize(const QPointer<IRenderer>& renderer, const SimulationData& data)
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
    this, &QmlGameState::forwardKeyEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerMouseEvent,
    this, &QmlGameState::forwardMouseEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerWheelEvent,
    this, &QmlGameState::forwardWheelEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragBegin,
    this, &QmlGameState::forwardDragBegin, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragMove,
    this, &QmlGameState::forwardDragMove, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragEnd,
    this, &QmlGameState::forwardDragEnd, Qt::BlockingQueuedConnection);
}

void QmlGameState::forwardKeyEvent(QKeyEvent* event, bool& accepted)
{
  accepted = onKeyEvent(event);
}

void QmlGameState::forwardMouseEvent(QMouseEvent* event, bool& accepted)
{
  accepted = onMouseEvent(event);
}

void QmlGameState::forwardWheelEvent(QWheelEvent* event, bool& accepted)
{
  accepted = onWheelEvent(event);
}

void QmlGameState::forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
  onDragBegin(button, origin);
}

void QmlGameState::forwardDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
  onDragMove(button, origin, position, change);
}

void QmlGameState::forwardDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
  onDragEnd(button, origin, position);
}

}
