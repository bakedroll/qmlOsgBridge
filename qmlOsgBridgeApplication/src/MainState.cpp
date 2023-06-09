#include "MainState.h"

#include <QPointer>

#include <osg/ShapeDrawable>

#include <osgHelper/LightingNode.h>

#include <qmlOsgBridge/IQmlGameProxy.h>

MainState::MainState(osgHelper::ioc::Injector& injector) :
  QmlGameState(injector),
  m_hdr(injector.inject<osgHelper::ppu::HDR>())
{
  enableEventHandling();
}

MainState::~MainState() = default;

void MainState::onInitialize(const QPointer<qmlOsgBridge::IQmlGameProxy>& proxy, const SimulationData& data)
{
  const auto view = proxy->getView();

  auto camera = view->getCamera(osgHelper::View::CameraType::Scene);
  camera->setPosition(osg::Vec3f(0, -5, 0));
  camera->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));

  const auto drawable = new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 1.0));

  m_transform = new osg::PositionAttitudeTransform();
  m_transform->setPosition(osg::Vec3d(0.0, 0.0, -5.0));
  m_transform->addChild(drawable);

  auto lightingNode = new osgHelper::LightingNode();
  lightingNode->addChild(m_transform);

  lightingNode->getLightModel()->setAmbientIntensity(osg::Vec4(0.5f, 0.5f, 0.5f, 1.0f));

  lightingNode->setLightEnabled(0, true);

  const auto light = lightingNode->getOrCreateLight(0);

  light->setDiffuse(osg::Vec4(2.0, 2.0, 2.0, 1.0));
  light->setSpecular(osg::Vec4(1.0, 1.0, 1.0, 1.0));
  light->setAmbient(osg::Vec4(0.0, 0.0, 0.0, 1.0));

  view->getRootGroup()->addChild(lightingNode);

  view->addPostProcessingEffect(m_hdr, true, m_hdr->Name);
}

void MainState::onUpdate(const SimulationData& data)
{
  m_transform->setPosition(osg::Vec3d(0.0, 3.0 - 5.0 * cos(data.time), 0.0));
}

bool MainState::onKeyEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape)
  {
    requestExitEventState(ExitGameStateMode::ExitAll);
    return true;
  }
  return false;
}
