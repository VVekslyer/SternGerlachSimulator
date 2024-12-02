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
        columns: 12  // Increased since tiles are smaller

        Repeater {
            model: 144   // Increased grid capacity
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
        width: 72  // Was 96
        spacing: -4

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile {
                colorKey: "red"
                axisLabel: "x"
            }
        }

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile {
                colorKey: "blue"
                axisLabel: "z"
            }
        }

        Repeater {
            model: [0, 1, 2, 3, 4, 5, 6]
            delegate: ConnectionLine { }
        }

        Repeater {
            model: [0, 1, 2, 3]
            delegate: Wall { }
        }
    }
}
