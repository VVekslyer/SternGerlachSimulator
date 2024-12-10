/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  tiles/Output.qml
 */

import QtQuick 2.15
import SternGerlach 1.0

Item {
    id: root

    required property string text
    required property int modelData
    required property var simulator

    width: 72/2
    height: 72/2

    // Debug at creation
    Component.onCompleted: {
        console.debug("[Output] Component created")
        console.debug("[Output] Simulator:", simulator)
        console.debug("[Output] Initial values:", {
            throughput: simulator?.throughputPercent,
            particleSum: simulator?.particleSum,
            particleCount: simulator?.particleCount
        })
    }

    // Monitor simulator property
    onSimulatorChanged: {
        console.debug("[Output] Simulator changed:", simulator)
    }

    // Monitor values
    onThroughputChanged: {
        console.debug("[Output] Throughput changed to:", throughput)
    }

    // Update property bindings with debug
    readonly property real throughput: {
        let value = simulator?.throughputPercent ?? 0.0
        console.debug("[Output] Throughput binding evaluated:", value)
        return value
    }
    readonly property real upPercentage: simulator?.upPercent ?? 0.0
    readonly property real downPercentage: simulator?.downPercent ?? 0.0
    readonly property int particleSum: simulator?.particleSum ?? 0
    readonly property int particleCount: simulator?.particleCount ?? 0
    readonly property int upCount: simulator?.upCount ?? 0
    readonly property int downCount: simulator?.downCount ?? 0

    // Enhanced connections
    Connections {
        target: simulator
        enabled: simulator !== null

        function onResultsChanged() {
            console.debug("[Output] Results changed signal received")
            console.debug("[Output] New values:", {
                throughput: simulator.throughputPercent,
                particleSum: simulator.particleSum
            })
            root.update()
        }
    }

    function update() {
        tile.update()
    }

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
                text: "out"
            }

            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 18
            Drag.hotSpot.y: 18
            Drag.keys: [ "output" ]

            function update() {
                resultColumn.visible = false
                resultColumn.visible = true
            }

            // Results display
            Rectangle {
                x: parent.width - 5
                y: -parent.height/2
                visible: true // Force visible
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
                    visible: true // Force visible

                    Text {
                        text: `∑ ${throughput.toFixed(1)}% (${particleSum}/${particleCount})`
                        color: "white"
                        font.pixelSize: 10
                        visible: true
                    }
                    Text {
                        text: `↑ ${upPercentage.toFixed(1)}% (${upCount}/${particleCount})`
                        color: "white"
                        font.pixelSize: 10
                        visible: true
                    }
                    Text {
                        text: `↓ ${downPercentage.toFixed(1)}% (${downCount}/${particleCount})`
                        color: "white"
                        font.pixelSize: 10
                        visible: true
                    }
                }
            }

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
