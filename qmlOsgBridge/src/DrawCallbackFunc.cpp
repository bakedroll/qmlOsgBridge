#include "DrawCallbackFunc.h"

namespace qmlOsgBridge
{

DrawCallbackFunc::DrawCallbackFunc(const std::function<void()>& func)
  : DrawCallback()
  , m_func(func)
{
}

DrawCallbackFunc::~DrawCallbackFunc() = default;

void DrawCallbackFunc::operator()(osg::RenderInfo& renderInfo) const
{
  m_func();
}

}
