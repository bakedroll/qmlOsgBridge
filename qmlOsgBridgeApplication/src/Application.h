#pragma once

#include <qmlOsgBridge/QmlGameApplication.h>

class Application : public qmlOsgBridge::QmlGameApplication
{
  Q_OBJECT

public:
  Application(int& argc, char** argv);
  ~Application() override;

protected:
  void registerComponents(osgHelper::ioc::InjectionContainer& container) override;

};
