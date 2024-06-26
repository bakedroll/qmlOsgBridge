#pragma once

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QtUtilsLib/MultithreadedApplication.h>

#include <libQtGame/GameStatesApplication.h>

namespace qmlOsgBridge
{

class IQmlContext;
class QmlGameState;

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

    addQmlImportPaths();
    m_qmlEngine.load(QUrl(rootQmlFilename));
    if (!m_warnings.empty() || !injectPushAndPrepareState<TState>())
    {
      return -1;
    }

    return GameStatesApplication::runGame();
  }

  void disableVSync();

protected:
  void onInitialize(const osg::ref_ptr<libQtGame::GameUpdateCallback>& updateCallback) override;
  void onPrepareGameState(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state,
    const libQtGame::AbstractGameState::SimulationData& simData) override;
  void onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state) override;
  void onEmptyStateList() override;
  void onShutdown() override;
  void onPreStatesUpdate(const osgHelper::SimulationCallback::SimulationData& data) override;

  int execApp() override;
  void quitApp() override;

  bool notify(QObject *receiver, QEvent *event) override;

  virtual std::vector<QString> qmlImportPaths() const;

private Q_SLOTS:
  void receiveWarnings(const QList<QQmlError>& warnings);

private:
  static const char* s_qmlUri;
  static const char* s_contextName;
  static const char* s_viewportName;
  static const int s_majorVersion;
  static const int s_minorVersion;

  QQmlApplicationEngine m_qmlEngine;
  osg::ref_ptr<IQmlContext> m_qmlContext;

  QList<QQmlError> m_warnings;

  std::vector<osg::ref_ptr<QmlGameState>> m_qmlGameStates;

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

  void addQmlImportPaths();

};

}
