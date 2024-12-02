// tiles/DropTile.qml
import QtQuick 2.15

DropArea {
    id: dragTarget

    property string colorKey

    width: 44  // Was 58
    height: 36 // Was 48
    keys: colorKey == "any" ? ["red", "blue", "orange", "white", "line", "wall", "output"] : [ colorKey ]

    Rectangle {
        id: dropRectangle

        anchors.fill: parent
        color: dragTarget.containsDrag ? "grey" : (colorKey == "any" ? "white" : dragTarget.colorKey)
        //opacity: 0 // clear out the grey color
    }
}
