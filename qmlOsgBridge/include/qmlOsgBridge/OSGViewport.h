#pragma once

#include <qmlOsgBridge/IOSGViewport.h>

#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QPointer>
#include <QQuickItem>
#include <QSGSimpleTextureNode>
#include <QSGTexture>

#include <osgHelper/View.h>

#include <memory>

namespace qmlOsgBridge
{

class IRenderer;
class IWindow;

class OSGViewport : public QQuickItem,
                    public IOSGViewport
{
  Q_OBJECT
public:
  explicit OSGViewport(QQuickItem* parent = nullptr);
  ~OSGViewport() override;

  Q_PROPERTY(QPointer<IRenderer> renderer READ renderer WRITE setRenderer REQUIRED)

  void acceptWindow(IWindow* window);

  void prepareNode() override;
  void deleteFrameBufferObjects() override;

  osg::ref_ptr<osgViewer::View> getView() const override;

  QPointer<IRenderer> renderer() const;
  void setRenderer(const QPointer<IRenderer>& renderer);

protected:
  QSGNode* updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* updatePaintNodeData) override;
  void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override;
  void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData& value) override;

  void mousePressEvent(QMouseEvent* event) override;

private:
  IWindow* m_window;
  QPointer<IRenderer> m_renderer;

  QOpenGLFramebufferObjectFormat m_format;
  std::unique_ptr<QOpenGLFramebufferObject> m_renderFbo;
  std::unique_ptr<QOpenGLFramebufferObject> m_displayFbo;

  QPointer<QSGTexture> m_renderTexture;
  QPointer<QSGTexture> m_displayTexture;
  QSGSimpleTextureNode* m_textureNode;

  osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_context;

  int m_remainingSizeUpdateSteps;
  QSize m_renderSize;

  bool m_needPrepareNodesUpdate;

  void preDrawFunction();
  void postDrawFunction();

  void updateViewport();

};

}
