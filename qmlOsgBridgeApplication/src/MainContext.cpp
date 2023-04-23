#include "MainContext.h"

#include <qmlOsgBridge/DefaultRenderer.h>

#include <osgHelper/ppu/HDR.h>

MainContext::MainContext(osgHelper::ioc::Injector& injector) :
  IQmlContext(),
  m_renderer(std::make_unique<qmlOsgBridge::DefaultRenderer>()),
  m_isFullscreen(false)
{
}

MainContext::~MainContext() = default;

QPointer<qmlOsgBridge::IRenderer> MainContext::getMainRenderer() const
{
  return m_renderer.get();
}

void MainContext::onGameStateAction(const osg::ref_ptr<libQtGame::AbstractGameState>& state, ActionType type)
{
}

void MainContext::onToggleHDR()
{
  /*m_renderer->dispatchRenderThread([this]()
  {
    const auto view = m_renderer->getView();
    const auto isEnabled = view->getPostProcessingEffectEnabled(osgHelper::ppu::HDR::Name);
    view->setPostProcessingEffectEnabled(osgHelper::ppu::HDR::Name, !isEnabled);
  });*/
}

void MainContext::onToggleFullscreen()
{
  m_isFullscreen = !m_isFullscreen;
  Q_EMIT changedFullscreen();
}

QWindow::Visibility MainContext::windowVisibility() const
{
  return m_isFullscreen ? QWindow::Visibility::FullScreen : QWindow::Visibility::Windowed;
}

Qt::WindowFlags MainContext::windowFlags()
{
  return m_isFullscreen ? Qt::FramelessWindowHint | Qt::Window : Qt::Window;
}
