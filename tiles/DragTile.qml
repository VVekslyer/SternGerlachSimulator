// tiles/DragTile.qml
import QtQuick 2.15

Item {
    id: root

    required property string colorKey
    required property int modelData

    width: 128  // Doubled
    height: 128 // Doubled

    MouseArea {
        id: mouseArea

        width: 144  // Doubled (72 * 2)
        height: 128 // Doubled
        anchors.centerIn: parent

        drag.target: tile

        onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

        Rectangle {
            id: tile

            width: 156  // Doubled (78 * 2)
            height: 128 // Doubled
            anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
            }
            transform: Translate { x: 39; y: 32 }  // Move rectangle right and down

            color: root.colorKey

            Drag.keys: [ root.colorKey ]
            Drag.active: mouseArea.drag.active
            Drag.hotSpot.x: 64  // Adjusted for new center
            Drag.hotSpot.y: 64  // Adjusted for new center

            Text {
            anchors.fill: parent
            color: "white"
            font.pixelSize: 96  // Increased for larger tile
            textFormat: Text.RichText
            text: "SG<small>z</small>"
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
