#include <qmlOsgBridge/EventProcessingState.h>

namespace qmlOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector) :
  AbstractGameState(injector),
  m_eventHandler(std::make_unique<libQtGame::KeyboardMouseEventFilter>())
{
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerKeyEvent,
    this, &EventProcessingState::forwardKeyEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerMouseEvent,
    this, &EventProcessingState::forwardMouseEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerWheelEvent,
    this, &EventProcessingState::forwardWheelEvent, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragBegin,
    this, &EventProcessingState::forwardDragBegin, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragMove,
    this, &EventProcessingState::forwardDragMove, Qt::BlockingQueuedConnection);
  connect(m_eventHandler.get(), &libQtGame::KeyboardMouseEventFilter::triggerDragEnd,
    this, &EventProcessingState::forwardDragEnd, Qt::BlockingQueuedConnection);
}

EventProcessingState::~EventProcessingState() = default;

void EventProcessingState::onInitialize(const QPointer<IRenderer>& renderer, const SimulationData& data)
{
}

libQtGame::KeyboardMouseEventFilter& EventProcessingState::eventHandler() const
{
  return *m_eventHandler;
}

void EventProcessingState::onResizeEvent(QResizeEvent* event)
{
}

bool EventProcessingState::onKeyEvent(QKeyEvent* event)
{
  return false;
}

bool EventProcessingState::onMouseEvent(QMouseEvent* event)
{
  return false;
}

bool EventProcessingState::onWheelEvent(QWheelEvent* event)
{
  return false;
}

void EventProcessingState::onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
}

void EventProcessingState::onDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
}

void EventProcessingState::onDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
}

void EventProcessingState::forwardKeyEvent(QKeyEvent* event, bool& accepted)
{
  accepted = onKeyEvent(event);
}

void EventProcessingState::forwardMouseEvent(QMouseEvent* event, bool& accepted)
{
  accepted = onMouseEvent(event);
}

void EventProcessingState::forwardWheelEvent(QWheelEvent* event, bool& accepted)
{
  accepted = onWheelEvent(event);
}

void EventProcessingState::forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
  onDragBegin(button, origin);
}

void EventProcessingState::forwardDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
  onDragMove(button, origin, position, change);
}

void EventProcessingState::forwardDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
  onDragEnd(button, origin, position);
}

}
