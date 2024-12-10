/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  tiles/ConnectionLine.qml
 */

import QtQuick 2.15

Item {
    id: root

    width: 72  // Was 96
    height: 48 // Was 64

    MouseArea {
        id: mouseArea

        width: 59  // Was 78
        height: 48 // Was 64
        anchors.centerIn: parent

        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Rectangle {
            id: tile

            width: 88  // Was 58
            height: 3  // Was 4
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: 22
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
