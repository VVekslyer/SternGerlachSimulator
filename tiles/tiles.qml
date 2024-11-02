// tiles/tiles.qml
import QtQuick 2.15

Rectangle {
    id: root

    width: 1280
    height: 720
    color: "black"

    Grid {
        id: destination

        anchors {
            left: sourceColumn.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 5
        }
        width: parent.width - sourceColumn.width - 10
        height: parent.height - 10
        opacity: 0.5
        columns: 6

        Repeater {
            model: 36
            delegate: DropTile {
                colorKey: "any"
            }
        }
    }

    Column {
        id: sourceColumn

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 5
        }
        width: 64
        spacing: -4

        Repeater {
            model: 3
            delegate: DragTile {
                colorKey: "red"
                modelData: index
            }
        }
        Repeater {
            model: 3
            delegate: DragTile {
                colorKey: "blue"
                modelData: index
            }
        }
    }
}
