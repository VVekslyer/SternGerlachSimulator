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
        columns: 8  // Reduced columns to accommodate larger tiles
        // spacing: 2 // this is an option

        Repeater {
            model: 64  // Adjusted for new grid size
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
        width: 128  // Increased to accommodate larger tiles
        spacing: -4

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile_X {
                colorKey: "red"
            }
        }

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile {
                colorKey: "blue"
            }
        }

        Repeater {
            model: [0, 1, 2]
            delegate: ConnectionLine { }
        }
    }
}
