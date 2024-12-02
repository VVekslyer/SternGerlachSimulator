import QtQuick 2.15

Item {
    id: root

    required property string text
    required property int modelData

    width: 72/2
    height: 72/2

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
                anchors.fill: parent
                color: "white"
                font.pixelSize: 14
                text: root.text
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
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
