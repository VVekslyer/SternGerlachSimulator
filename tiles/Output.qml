/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  tiles/Output.qml
 */

import QtQuick 2.15
import SternGerlach 1.0  // Add import

Item {
    id: root

    required property string text
    required property int modelData
    required property var simulator

    width: 72/2
    height: 72/2

    // Initialize default values
    readonly property real throughput: simulator?.results?.throughputPercent ?? 0.0
    readonly property real upPercentage: simulator?.results?.upPercent ?? 0.0
    readonly property real downPercentage: simulator?.results?.downPercent ?? 0.0

    MouseArea {
        id: mouseArea
        width: 81/2
        height: 72/2
        anchors.centerIn: parent
        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Rectangle {
            id: tile
            width: 88/2
            height: 72/2
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }

            color: "transparent"
            border.color: "white"
            border.width: 2

            Text {
                id: outputText
                anchors.fill: parent
                color: "white"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                // Use property binding with static symbols
                text: {
                    if (!simulator) return "out";
                    return `${simulator.throughputPercent.toFixed(1)}%\n` +
                           `↑: ${simulator.upPercent.toFixed(1)}%\n` +
                           `↓: ${simulator.downPercent.toFixed(1)}%`;
                }
            }

            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 18
            Drag.hotSpot.y: 18
            Drag.keys: [ "output" ]

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

    Rectangle {
        x: parent.width - 5
        y: -5
        width: resultColumn.width + 10
        height: resultColumn.height + 6
        color: "#333333"
        radius: 3
        opacity: 0.8

        Column {
            id: resultColumn
            x: 5
            y: 3
            spacing: 1

            Text {
                text: "∑ " + throughput.toFixed(1) + "%"
                color: "white"
                font.pixelSize: 10
            }
            Text {
                text: "↑ " + upPercentage.toFixed(1) + "%"
                color: "white"
                font.pixelSize: 10
            }
            Text {
                text: "↓ " + downPercentage.toFixed(1) + "%"
                color: "white"
                font.pixelSize: 10
            }
        }
    }
}
