#include "Application.h"

#include "MainContext.h"
#include "MainState.h"

#include <osgHelper/ppu/HDR.h>

Application::Application(int& argc, char** argv) :
  QmlGameApplication(argc, argv)
{
}

Application::~Application() = default;

void Application::registerComponents(osgHelper::ioc::InjectionContainer& container)
{
  registerEssentialComponents(container);

  container.registerSingletonType<MainContext>();
  container.registerType<MainState>();

  container.registerType<osgHelper::ppu::HDR>();
}
