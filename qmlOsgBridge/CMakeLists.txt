begin_project(qmlOsgBridge LIBRARY)

enable_automoc()

require_library(Qt MODULES Core Qml Quick QuickControls2)
require_library(OpenSceneGraph)
require_library(osgPPU)

require_project(osgHelper PATH osgHelper)
require_project(utilsLib PATH utilsLib)
require_project(QtUtilsLib PATH QtUtilsLib)

add_source_directory(src)
add_source_directory(include)

add_include_directory(include)
