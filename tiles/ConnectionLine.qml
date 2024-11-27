// tiles/ConnectionLine.qml
import QtQuick 2.15

Item {
    id: root

    width: 64
    height: 64

    MouseArea {
        id: mouseArea

        width: 78
        height: 64
        anchors.centerIn: parent

        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Rectangle {
            id: tile

            width: 78
            height: 4
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            color: "white"

            Drag.keys: [ "white" ]
            Drag.active: mouseArea.drag.active
            // Drag.hotSpot.x: 32
            // Drag.hotSpot.y: 32

            states: State {
                when: mouseArea.drag.active
                AnchorChanges {
                    target: tile
                    anchors {
                        verticalCenter: undefined
                        horizontalCenter: undefined
                    }
                }
            }
        }
    }
}
