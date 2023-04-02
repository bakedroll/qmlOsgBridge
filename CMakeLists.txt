cmake_minimum_required(VERSION 3.4)

set(CMAKE_SCRIPTS_DIRECTORY "" CACHE STRING "The CMake scripts directory")
if(EXISTS ${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
  include(${CMAKE_SCRIPTS_DIRECTORY}/scripts.cmake)
else()
  message(FATAL_ERROR "Please specify CMAKE_SCRIPTS_DIRECTORY")
  return()
endif()

option(QT_USE_VERSION_5 "Use Qt version 5" ON)

project(qmlOsgBridge)

add_subdirectory(qmlOsgBridge)
add_subdirectory(qmlOsgBridgeApplication)

make_projects()

if (MSVC)
  osg_copy_additional_binaries(zlib)

  qt_copy_additional_binaries(
    Qt5QuickTemplates2
    Qt5Network
    Qt5QmlModels
    Qt5QmlWorkerScript)

  qt_copy_plugins(
    platforms/qwindows)

  qt_copy_qml_binaries(
    QtQuick/Window.2/windowplugin
    QtQuick/Controls.2/qtquickcontrols2plugin
    QtQuick.2/qtquick2plugin)
endif()