#pragma once

#include <osg/Camera>

#include <functional>

namespace qmlOsgBridge
{

class DrawCallbackFunc : public osg::Camera::DrawCallback
{
public:
  explicit DrawCallbackFunc(const std::function<void()>& func);
  ~DrawCallbackFunc() override;

  void operator()(osg::RenderInfo& renderInfo) const override;

private:
  std::function<void()> m_func;

};

}
