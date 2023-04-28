#pragma once

#include <osg/Referenced>

#include <QObject>
#include <QPointer>

#include <libQtGame/AbstractGameState.h>

namespace qmlOsgBridge
{

class IRenderer;

class IQmlContext : public QObject,
                    public osg::Referenced
{
  Q_OBJECT

  Q_PROPERTY(QPointer<IRenderer> renderer READ getMainRenderer CONSTANT)

public:
  enum class ActionType
  {
    Enter,
    Exit
  };

  IQmlContext() = default;
  ~IQmlContext() override = default;

  virtual QPointer<IRenderer> getMainRenderer() const = 0;

  virtual void onGameStateAction(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state, ActionType type) = 0;

};

}
