begin_project(qmlOsgBridgeApplication EXECUTABLE OPTIONAL)

enable_automoc()
enable_autorcc()

require_library(Qt MODULES Core Qml Quick QuickControls2)
require_library(OpenSceneGraph MODULES osg osgViewer osgUtil osgGA osgDB osgText OpenThreads)
require_library(osgPPU)

require_project(osgHelper PATH osgHelper)
require_project(utilsLib PATH utilsLib)
require_project(QtUtilsLib PATH QtUtilsLib)
require_project(qmlOsgBridge)

add_source_directory(src)

add_qrc_files(resources.qrc)
