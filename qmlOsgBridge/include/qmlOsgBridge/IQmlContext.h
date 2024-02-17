#pragma once

#include <osg/Referenced>

#include <QObject>
#include <QPointer>

#include <libQtGame/AbstractGameState.h>

namespace qmlOsgBridge
{

class IQmlGameProxy;

class IQmlContext : public QObject,
                    public osg::Referenced
{
  Q_OBJECT

  Q_PROPERTY(QPointer<IQmlGameProxy> proxy READ getQmlGameProxy CONSTANT)

public:
  enum class ActionType
  {
    Enter,
    Exit
  };

  IQmlContext() = default;
  ~IQmlContext() override = default;

  virtual QPointer<IQmlGameProxy> getQmlGameProxy() const = 0;

  virtual void onGameStateAction(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state, ActionType type) = 0;

  virtual void onShutdown() = 0;

};

}
