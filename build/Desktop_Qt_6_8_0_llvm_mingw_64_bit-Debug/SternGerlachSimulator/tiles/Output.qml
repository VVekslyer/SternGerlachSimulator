import QtQuick 2.15
import SternGerlach 1.0  // Add import

Item {
    id: root

    required property string text
    required property int modelData

    width: 72/2
    height: 72/2

    // Reference simulator from root
    property var simulator: Window.window ? Window.window.simulator : null

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

                text: {
                    if (!simulator) return "out";
                    let results = simulator.getResults();
                    return (results.throughputPercent).toFixed(1) + "%\n" +
                           "↑:" + (results.upPercent).toFixed(1) + "%\n" +
                           "↓:" + (results.downPercent).toFixed(1) + "%";
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
}
