#pragma once

#include <QSize>

#include <osgGA/EventQueue>
#include <osgViewer/CompositeViewer>

namespace qmlOsgBridge
{

class IOSGViewport
{
public:
  IOSGViewport() = default;
  virtual ~IOSGViewport() = default;

  virtual osg::ref_ptr<osgViewer::CompositeViewer> getViewer() const = 0;
  virtual osg::ref_ptr<osgViewer::GraphicsWindow> getGraphicsWindow() const = 0;

  virtual QSize getSize() const = 0;
  virtual osg::ref_ptr<osgGA::EventQueue> getPendingEvents() const = 0;

};

}
