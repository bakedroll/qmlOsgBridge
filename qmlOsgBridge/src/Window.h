#pragma once

#include <QObject>
#include <QThread>

#include <osgViewer/CompositeViewer>

#include "IWindow.h"

#include <set>

namespace qmlOsgBridge
{

class IOSGViewport;
class RenderWorker;

class Window : public QObject,
               public IWindow
{
  Q_OBJECT

public:
  explicit Window(QQuickWindow* quickWindow);
  ~Window() override;

  QPointer<QQuickWindow> getQuickWindow() const override;

  int getMinFrameTimeMs() const override;

  void addViewport(IOSGViewport& viewport) override;
  void removeViewport(IOSGViewport& viewport) override;

  QPointer<QThread> getRenderThread() const override;

  static Window* fromQuickWindow(QQuickWindow* quickWindow);
  static void closeAll();

public Q_SLOTS:
  void prepareNodes();
  // TODO: needed?
  //void onSceneGraphAboutToStop();

Q_SIGNALS:
  void pendingNewTexture();
  void renderThreadChanged(QThread* renderThread);

private:
  static std::map<QQuickWindow*, Window*> m_windowsStorage;

  QQuickWindow* m_quickWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;
  QPointer<QThread> m_renderThread;

  std::set<IOSGViewport*> m_viewports;

};

}
