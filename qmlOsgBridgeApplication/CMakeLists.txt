begin_project(qmlOsgBridgeApplication EXECUTABLE OPTIONAL)

enable_automoc()
enable_autorcc()

require_library(Qt MODULES Core Qml Quick QuickControls2)
require_library(OpenSceneGraph MODULES osg osgViewer osgUtil osgGA osgDB osgText OpenThreads)
require_library(osgPPU)

require_project(qmlOsgBridge)

add_source_directory(src)
add_other_directory(resources)

add_qrc_files(resources.qrc)
