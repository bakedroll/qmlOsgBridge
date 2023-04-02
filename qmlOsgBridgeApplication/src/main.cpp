#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>

int main(int argc, char** argv)
{
  //QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
  QGuiApplication app(argc, argv);

  QQuickStyle::setStyle("Fusion");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/resources/mainwindow.qml")));

  return app.exec();
}
