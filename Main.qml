// Main.qml
import QtQuick
import QtQuick.Window

Window {
    id: mainWindow
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Stern-Gerlach Simulator")

    // Correct way to include tiles.qml using Loader
    Loader {
        id: tilesLoader
        anchors.fill: parent
        source: "tiles/tiles.qml"
    }
}
