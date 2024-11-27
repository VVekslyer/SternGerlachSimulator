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
        columns: 10

        Repeater {
            model: 80
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

        // Updated Repeater for red tiles
        Repeater {
            model: [0, 1, 2]  // Using an array as the model
            delegate: DragTile_X {
                colorKey: "red"
            }
        }

        // Updated Repeater for blue tiles
        Repeater {
            model: [0, 1, 2]  // Using an array as the model
            delegate: DragTile {
                colorKey: "blue"
            }
        }

        // New Repeater for ConnectionLine
        Repeater {
            model: [0, 1, 2]
            delegate: ConnectionLine { }
        }
    }
}