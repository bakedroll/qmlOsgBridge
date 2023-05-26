#include "RendererImpl.h"

#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>

#include <qmlOsgBridge/IOSGViewport.h>

namespace qmlOsgBridge
{

RendererImpl::RendererImpl(const IOSGViewport& viewport, const QPointer<IQmlGameProxy>& proxy) :
  m_viewport(viewport),
  m_proxy(proxy),
  m_isDefaultFboSet(false)
{
  updateSize();
}

RendererImpl::~RendererImpl() = default;

void RendererImpl::synchronize(QQuickFramebufferObject* item)
{
  osgGA::EventQueue::Events events;
  m_viewport.getPendingEvents()->takeEvents(events);
  m_proxy->getView()->getEventQueue()->appendEvents(events);
}

void RendererImpl::render()
{
  m_proxy->executeMutexLocked([this]()
  {
    if (!m_isDefaultFboSet)
    {
      m_viewport.getGraphicsWindow()->setDefaultFboId(framebufferObject()->handle());
      m_isDefaultFboSet = true;
    }

    QOpenGLContext::currentContext()->functions()->glUseProgram(0);
    if (sizeHasChanged())
    {
      updateSize();
    }

    m_viewport.getViewer()->frame();
  });
}

QOpenGLFramebufferObject* RendererImpl::createFramebufferObject(const QSize& size)
{
  return new QOpenGLFramebufferObject(size, QOpenGLFramebufferObjectFormat());
}

bool RendererImpl::sizeHasChanged() const
{
  const auto t = m_viewport.getGraphicsWindow()->getTraits();
  const auto size = m_viewport.getSize();

  return t->width != size.width() || t->height != size.height();
}

void RendererImpl::updateSize()
{
  const auto size = m_viewport.getSize();
  m_viewport.getGraphicsWindow()->resized(0, 0, size.width(), size.height());
  m_proxy->getView()->updateResolution(osg::Vec2i(size.width(), size.height()));
}

}
