#pragma once

#include <libQtGame/AbstractGameState.h>

#include <libQtGame/KeyboardMouseEventFilter.h>

#include <qmlOsgBridge/IQmlGameProxy.h>

#include <memory>

namespace qmlOsgBridge
{

class QmlGameState : public libQtGame::AbstractGameState
{
public:
  QmlGameState(osgHelper::ioc::Injector& injector);
  ~QmlGameState() override;

  virtual void onInitialize(const QPointer<IQmlGameProxy>& proxy, const SimulationData& data);

  bool isEventHandlingEnabled() const;
  libQtGame::KeyboardMouseEventFilter& eventHandler() const;

  void setInitialized();
  bool isInitialized() const;

protected:
  virtual bool onKeyEvent(QKeyEvent* event);
  virtual bool onMouseEvent(QMouseEvent* event);
  virtual bool onWheelEvent(QWheelEvent* event);

  virtual void onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  virtual void onDragMove(
    Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position, const osg::Vec2f& change);
  virtual void onDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

  void enableEventHandling();

private Q_SLOTS:
  void forwardKeyEvent(QKeyEvent* event, bool& accepted);
  void forwardMouseEvent(QMouseEvent* event, bool& accepted);
  void forwardWheelEvent(QWheelEvent* event, bool& accepted);

  void forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  void forwardDragMove(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position, const osg::Vec2f& change);
  void forwardDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

private:
  std::unique_ptr<libQtGame::KeyboardMouseEventFilter> m_eventHandler;
  bool m_isInitialized;

};

}
