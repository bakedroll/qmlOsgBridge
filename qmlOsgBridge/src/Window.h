#pragma once

#include <QObject>
#include <QPointer>
#include <QQuickWindow>
#include <QThread>

#include <osgViewer/CompositeViewer>

#include "IWindow.h"

#include <memory>
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

  void flush() override;
  void frame() override;
  void deleteFbos() override;

  void addViewport(IOSGViewport& viewport) override;
  void removeViewport(IOSGViewport& viewport) override;

  bool initializeRenderContextIfNecessary() override;
  void dispatchRenderThread(const std::function<void()>& func) override;
  void dispatchRenderThreadBlocking(const std::function<void()>& func) override;

  QPointer<QThread> getRenderThread() const override;

  static IWindow* fromQuickWindow(QQuickWindow* quickWindow);
  static void closeAll();

public Q_SLOTS:
  void ready();
  void newTexture();
  void prepareNodes();

Q_SIGNALS:
  void textureInUse();
  void triggerDispatchRenderThread(const std::function<void()>& func);
  void triggerDispatchRenderThreadBlocking(const std::function<void()>& func);

private:
  static std::map<QQuickWindow*, IWindow*> m_windowsStorage;

  QQuickWindow* m_quickWindow;
  osg::ref_ptr<osgViewer::CompositeViewer> m_viewer;

  std::set<IOSGViewport*> m_viewports;

  QPointer<QThread> m_renderThread;
  std::unique_ptr<RenderWorker> m_renderWorker;

  bool m_isNewTexture;

};

}