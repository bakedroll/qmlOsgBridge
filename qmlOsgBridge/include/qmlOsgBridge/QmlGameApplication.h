#pragma once

#include <libQtGame/GameStatesApplication.h>

#include <QtUtilsLib/MultithreadedApplication.h>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

namespace qmlOsgBridge
{

class IQmlContext;

class QmlGameApplication : public QtUtilsLib::MultithreadedApplication<QGuiApplication>,
                           public libQtGame::GameStatesApplication
{
public:
  QmlGameApplication(int& argc, char** argv);
  ~QmlGameApplication();

  template <typename TState, typename TContext, typename = typename std::enable_if<
    std::is_base_of<libQtGame::AbstractGameState, TState>::value &&
    std::is_base_of<IQmlContext, TContext>::value>::type>
  int runGame(const QString& rootQmlFilename)
  {
    setupIOC();

    if (!injectAndRegisterContext<TContext>())
    {
      return -1;
    }

    m_qmlEngine.load(QUrl(rootQmlFilename));
    if (!injectPushAndPrepareState<TState>())
    {
      return -1;
    }

    return GameStatesApplication::runGame();
  }

protected:
  void onInitialize(const osg::ref_ptr<libQtGame::GameUpdateCallback>& updateCallback) override;
  void onPrepareGameState(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state,
    const libQtGame::AbstractGameState::SimulationData& simData) override;
  void onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state) override;
  void onEmptyStateList() override;
  void onShutdown() override;

  int execApp() override;
  void quitApp() override;

  bool notify(QObject *receiver, QEvent *event) override;

private:
  static const char* s_qmlUri;
  static const char* s_contextName;
  static const int s_majorVersion;
  static const int s_minorVersion;

  QQmlApplicationEngine m_qmlEngine;
  osg::ref_ptr<IQmlContext> m_qmlContext;

  template <typename TContext>
  bool injectAndRegisterContext()
  {
    auto context = injector().inject<TContext>();
    assert_return(context.valid(), false);

    m_qmlContext = context;

    qmlRegisterSingletonInstance<TContext>(
      s_qmlUri, s_majorVersion, s_minorVersion, s_contextName, context.get());
    return true;
  }

};

}
