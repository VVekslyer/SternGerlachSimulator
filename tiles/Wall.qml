/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  tiles/Wall.qml
 */

import QtQuick 2.15

Item {
    id: root

    width: 72
    height: 48

    MouseArea {
        id: mouseArea

        width: 59
        height: 72
        anchors.centerIn: parent

        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Item {
            id: tile

            width: 44
            height: 48
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            // Vertical rectangle
            Rectangle {
                width: 3
                height: 40
                color: "white"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                    horizontalCenterOffset: 44
                }

                Drag.hotSpot.x: width/2   // Center horizontally
                Drag.hotSpot.y: height/2  // Center vertically
            }

            // Horizontal rectangle
            Rectangle {
                width: 66
                height: 3
                color: "white"
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }
            }

            // Upper diagonal
            Rectangle {
                width: 14
                height: 3
                color: "white"
                transform: Rotation { origin.x: 0; origin.y: 0; angle: -45 }
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    horizontalCenterOffset: 6 + 44
                    top: parent.top
                    topMargin: 18
                }
            }

            // Lower diagonal
            Rectangle {
                width: 14
                height: 3
                color: "white"
                transform: Rotation { origin.x: 0; origin.y: 0; angle: -45 }
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    horizontalCenterOffset: 6 + 44
                    bottom: parent.bottom
                    bottomMargin: 8
                }
            }

            Drag.keys: [ "wall" ]
            Drag.active: mouseArea.drag.active

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