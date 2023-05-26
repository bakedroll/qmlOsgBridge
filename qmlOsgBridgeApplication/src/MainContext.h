#pragma once

#include <qmlOsgBridge/IQmlContext.h>

#include <osgHelper/ioc/Injector.h>

#include <QWindow>

#include <memory>

class MainContext : public qmlOsgBridge::IQmlContext
{
  Q_OBJECT

public:
  explicit MainContext(osgHelper::ioc::Injector& injector);
  ~MainContext() override;

  Q_PROPERTY(QWindow::Visibility windowVisibility READ windowVisibility NOTIFY changedFullscreen)
  Q_PROPERTY(Qt::WindowFlags windowFlags READ windowFlags NOTIFY changedFullscreen)

  QPointer<qmlOsgBridge::IQmlGameProxy> getQmlGameProxy() const override;

  void onGameStateAction(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state, ActionType type) override;

  Q_INVOKABLE
  void onToggleHDR();

  Q_INVOKABLE
  void onToggleFullscreen();

  QWindow::Visibility windowVisibility() const;
  Qt::WindowFlags windowFlags();

Q_SIGNALS:
  void changedFullscreen();

private:
  std::unique_ptr<qmlOsgBridge::IQmlGameProxy> m_proxy;

  bool m_isFullscreen;

};
