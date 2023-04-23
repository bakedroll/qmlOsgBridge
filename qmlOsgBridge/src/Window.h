#pragma once

#include <QObject>

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

  void frame() override;
  int getMinFrameTimeMs() const override;

  void addViewport(IOSGViewport& viewport) override;
  void removeViewport(IOSGViewport& viewport) override;

  QPointer<QThread> getRenderThread() const override;

  static IWindow* fromQuickWindow(QQuickWindow* quickWindow);
  static void closeAll();

public Q_SLOTS:
  void prepareNodes();
  // TODO: needed?
  //void onSceneGraphAboutToStop();

Q_SIGNALS:
  void pendingNewTexture();

private:
  static std::map<QQuickWindow*, IWindow*> m_windowsStorage;

  QQuickWindow* m_quickWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;

  std::set<IOSGViewport*> m_viewports;

  bool m_isNewTexture;
  bool m_isInitialized;

};

}