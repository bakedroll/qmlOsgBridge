#pragma once

#include <osgViewer/View>

#include <QObject>
#include <QPointer>

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
