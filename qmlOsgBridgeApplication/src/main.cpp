#include "Application.h"
#include "MainContext.h"
#include "MainState.h"

int main(int argc, char** argv)
{
  QGuiApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

  Application app(argc, argv);
  return app.runGame<MainState, MainContext>("qrc:/resources/mainwindow.qml");
}
