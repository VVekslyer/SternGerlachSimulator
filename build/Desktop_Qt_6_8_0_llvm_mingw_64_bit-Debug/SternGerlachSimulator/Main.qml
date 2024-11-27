// Main.qml
import QtQuick
import QtQuick.Window

Window {
    id: mainWindow
    width: 1280
    height: 720
    visible: true
    title: qsTr("Stern-Gerlach Simulator")

    // Correct way to include tiles.qml using Loader
    Loader {
        id: tilesLoader
        anchors.fill: parent
        source: "tiles/tiles.qml"
    }
}
