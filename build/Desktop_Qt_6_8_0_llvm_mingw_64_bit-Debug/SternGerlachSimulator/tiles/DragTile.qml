// tiles/DragTile.qml
import QtQuick 2.15

Item {
    id: root

    required property string colorKey
    required property int modelData
    required property string axisLabel  // New property for x/z

    width: 72  // Was 96 (75%)
    height: 72 // Was 96 (75%)

    MouseArea {
        id: mouseArea

        width: 81  // Was 108
        height: 72  // Was 96
        anchors.centerIn: parent

        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Rectangle {
            id: tile

            width: 88   // Was 117
            height: 72  // Was 96
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
            transform: Translate { x: 22; y: 18 }  // Was 29, 24

            color: root.colorKey

            Drag.keys: [ root.colorKey ]
            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 18  // Was 24
            Drag.hotSpot.y: 18  // Was 24

            Text {
                anchors.fill: parent
                color: "white"
                font.pixelSize: 54  // Was 72
                textFormat: Text.RichText
                text: "SG<small>" + root.axisLabel + "</small>"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
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
