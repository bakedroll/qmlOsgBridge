import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import qmlOsgBridge 1.0

Window {
    width: 640
    height: 480
    minimumWidth: 300
    minimumHeight: 300
    title: "qmlOsgBridgeApplication"
    visible: true

    visibility: Context.windowVisibility
    flags: Context.windowFlags

    OSGViewport {
        anchors.fill: parent
        renderer: Context.renderer

        Button {
            x: 50
            y: 50
            width: 200
            height: 48
            text: "Toggle HDR"
            onReleased: Context.onToggleHDR()
        }

        Button {
            x: 300
            y: 50
            width: 200
            height: 48
            text: "Toggle Fullscreen"
            onReleased: Context.onToggleFullscreen()
        }
    }
}
