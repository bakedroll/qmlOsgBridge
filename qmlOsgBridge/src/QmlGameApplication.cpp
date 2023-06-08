#include <qmlOsgBridge/QmlGameApplication.h>
#include <qmlOsgBridge/QmlGameState.h>

#include <qmlOsgBridge/IQmlContext.h>
#include <qmlOsgBridge/IQmlGameProxy.h>
#include <qmlOsgBridge/OSGViewport.h>

#include "WindowsStorage.h"

namespace qmlOsgBridge
{

const char* QmlGameApplication::s_qmlUri = "qmlOsgBridge";
const char* QmlGameApplication::s_contextName = "Context";
const char* QmlGameApplication::s_viewportName = "OSGViewport";
const int QmlGameApplication::s_majorVersion = 1;
const int QmlGameApplication::s_minorVersion = 0;

QmlGameApplication::QmlGameApplication(int& argc, char** argv) :
  MultithreadedApplication<QGuiApplication>(argc, argv),
  GameStatesApplication()
{
  qmlRegisterType<OSGViewport>(s_qmlUri, s_majorVersion, s_minorVersion, s_viewportName);
  qRegisterMetaType<QPointer<IQmlGameProxy>>("QPointer<IQmlGameProxy>");

  connect(&m_qmlEngine, &QQmlEngine::warnings, this, &QmlGameApplication::receiveWarnings, Qt::DirectConnection);
}

QmlGameApplication::~QmlGameApplication()
{
  if (m_qmlContext.valid())
  {
    m_qmlContext.release();
  }
}

void QmlGameApplication::onInitialize(const osg::ref_ptr<libQtGame::GameUpdateCallback>& updateCallback)
{
  const auto proxy = m_qmlContext->getQmlGameProxy();
  connect(proxy, &IQmlGameProxy::renderThreadPropagated, this, &QmlGameApplication::onRenderThreadPropagated,
    Qt::QueuedConnection);
  proxy->getView()->getSceneData()->addUpdateCallback(updateCallback);
}

void QmlGameApplication::onPrepareGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state,
  const libQtGame::AbstractGameState::SimulationData& simData)
{
  m_qmlContext->onGameStateAction(state, IQmlContext::ActionType::Enter);

  auto* eventState = dynamic_cast<QmlGameState*>(state.get());
  if (eventState)
  {
    const auto proxy = m_qmlContext->getQmlGameProxy();
    const auto renderThread = proxy->getRenderThread();
    if (renderThread)
    {
      prepareStateEventHandling(eventState, renderThread);
    }

    eventState->onInitialize(proxy, simData);
  }
}

void QmlGameApplication::onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state)
{
  auto* eventState = dynamic_cast<QmlGameState*>(state.get());
  if (eventState && eventState->isEventHandlingEnabled())
  {
    m_qmlContext->getQmlGameProxy()->getViewportQuickItem()->removeEventFilter(&eventState->eventHandler());
  }

  m_qmlContext->onGameStateAction(state, IQmlContext::ActionType::Exit);
}

void QmlGameApplication::onEmptyStateList()
{
  QtUtilsLib::Multithreading::executeInUiAsync([]()
  {
    WindowsStorage::get().closeAll();
  });
}

void QmlGameApplication::onShutdown()
{
  m_qmlContext->getQmlGameProxy()->getView()->cleanUp();
}

int QmlGameApplication::execApp()
{
  return exec();
}

void QmlGameApplication::quitApp()
{
  quit();
}

bool QmlGameApplication::notify(QObject* receiver, QEvent* event)
{
  if (safeExecute([&]() { QtUtilsLib::MultithreadedApplication<QGuiApplication>::notify(receiver, event); return 0; }))
  {
    return true;
  }

  return false;
}

std::vector<QString> QmlGameApplication::qmlImportPaths() const
{
  return {};
}

void QmlGameApplication::receiveWarnings(const QList<QQmlError>& warnings)
{
  m_warnings = warnings;
}

void QmlGameApplication::onRenderThreadPropagated(QThread* renderThread)
{
  QMutexLocker locker(&statesMutex());
  auto states = getGameStates();
  for (auto state : states)
  {
    auto* eventState = dynamic_cast<QmlGameState*>(state.get());
    if (eventState)
    {
      prepareStateEventHandling(eventState, renderThread);
    }
  }
}

void QmlGameApplication::addQmlImportPaths()
{
  const auto paths = qmlImportPaths();
  for (const auto& path: paths)
  {
    m_qmlEngine.addImportPath(path);
  }
}

void QmlGameApplication::prepareStateEventHandling(const osg::ref_ptr<QmlGameState>& state, const QPointer<QThread>& renderThread)
{
  if (state->isEventHandlingEnabled())
  {
    state->moveToThread(renderThread);
    m_qmlContext->getQmlGameProxy()->getViewportQuickItem()->installEventFilter(&state->eventHandler());
  }
}

}
