#pragma once

#include <osgViewer/View>

namespace qmlOsgBridge
{

class IOSGViewport
{
public:
  virtual ~IOSGViewport() = default;

  virtual void prepareNode() = 0;
  virtual void deleteFrameBufferObjects() = 0;

  virtual osg::ref_ptr<osgViewer::View> getView() const = 0;

};

}
