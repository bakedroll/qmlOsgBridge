#pragma once

#include <qmlOsgBridge/EventProcessingState.h>

#include <osg/PositionAttitudeTransform>

#include <osgHelper/ppu/HDR.h>

class MainState : public qmlOsgBridge::EventProcessingState
{
  Q_OBJECT

public:
  explicit MainState(osgHelper::ioc::Injector& injector);
  ~MainState() override;

  void onInitialize(const QPointer<qmlOsgBridge::IRenderer>& renderer, const SimulationData& data) override;
  void onUpdate(const SimulationData& data) override;

protected:
  bool onKeyEvent(QKeyEvent* event) override;

private:
  osg::ref_ptr<osg::PositionAttitudeTransform> m_transform;

  osg::ref_ptr<osgHelper::ppu::HDR> m_hdr;

};
