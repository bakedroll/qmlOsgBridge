#include <qmlOsgBridge/QmlGameApplication.h>
#include <qmlOsgBridge/EventProcessingState.h>

#include <QtUtilsLib/Multithreading.h>

#include <QSurfaceFormat>

#include <qmlOsgBridge/IQmlContext.h>
#include <qmlOsgBridge/OSGViewport.h>
#include <qmlOsgBridge/IRenderer.h>

#include "Window.h"

namespace qmlOsgBridge
{

const char* QmlGameApplication::s_qmlUri = "qmlOsgBridge";
const char* QmlGameApplication::s_contextName = "Context";
const int QmlGameApplication::s_majorVersion = 1;
const int QmlGameApplication::s_minorVersion = 0;

QmlGameApplication::QmlGameApplication(int& argc, char** argv) :
  MultithreadedApplication<QGuiApplication>(argc, argv),
  GameStatesApplication()
{
  auto format = QSurfaceFormat::defaultFormat();
  format.setSwapInterval(0);
  QSurfaceFormat::setDefaultFormat(format);

  qmlRegisterType<qmlOsgBridge::OSGViewport>(s_qmlUri, s_majorVersion, s_minorVersion, "OSGViewport");
  qRegisterMetaType<QPointer<IRenderer>>("QPointer<IRenderer>");
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
  m_qmlContext->getMainRenderer()->getView()->getSceneData()->addUpdateCallback(updateCallback);
}

void QmlGameApplication::onPrepareGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state,
  const libQtGame::AbstractGameState::SimulationData& simData)
{
  auto* eventState = dynamic_cast<EventProcessingState*>(state.get());
  if (eventState)
  {
    m_qmlContext->getMainRenderer()->moveObjectToRenderThread(eventState);
    m_qmlContext->getMainRenderer()->getQuickWindow()->installEventFilter(&eventState->eventHandler());

    eventState->onInitialize(m_qmlContext->getMainRenderer(), simData);
  }

  m_qmlContext->onGameStateAction(state, IQmlContext::ActionType::Enter);
}

void QmlGameApplication::onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state)
{
  auto* eventState = dynamic_cast<EventProcessingState*>(state.get());
  if (eventState)
  {
    m_qmlContext->getMainRenderer()->getQuickWindow()->removeEventFilter(&eventState->eventHandler());
  }

  m_qmlContext->onGameStateAction(state, IQmlContext::ActionType::Exit);
}

void QmlGameApplication::onEmptyStateList()
{
  QtUtilsLib::Multithreading::executeInUiAsync([]()
  {
    Window::closeAll();
  });
}

void QmlGameApplication::onShutdown()
{
  m_qmlContext->getMainRenderer()->getView()->cleanUp();
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

}
