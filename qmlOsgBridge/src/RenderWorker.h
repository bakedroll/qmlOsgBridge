#pragma once

#include <QElapsedTimer>
#include <QObject>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QPointer>
#include <QQuickWindow>
#include <QTimer>

namespace qmlOsgBridge
{

class IOSGViewport;
class IWindow;

class RenderWorker : public QObject
{
  Q_OBJECT

public:
  explicit RenderWorker(IWindow& window, QObject* parent = nullptr);
  ~RenderWorker() override;

  bool hasContext() const;

  void setupContext(const QPointer<QQuickWindow>& window, const QPointer<QThread>& renderThread);
  void setupSurface();

  void flush();

public Q_SLOTS:
  void render();
  void shutdown();
  void requestNext();
  void dispatch(const std::function<void()>& func);

Q_SIGNALS:
  void textureReady();

private:
  IWindow& m_window;

  QPointer<QOffscreenSurface> m_surface;
  QPointer<QOpenGLContext> m_context;

  QPointer<QTimer> m_nextFrameTimer;
  QElapsedTimer m_frameTimeMeasure;
  bool m_shuttingDown;

};

}
