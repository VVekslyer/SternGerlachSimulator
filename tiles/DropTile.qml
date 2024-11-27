// tiles/DropTile.qml
import QtQuick 2.15

DropArea {
    id: dragTarget

    property string colorKey

    width: 78
    height: 64
    keys: colorKey == "any" ? ["red", "blue", "white"] : [ colorKey ]

    Rectangle {
        id: dropRectangle

        anchors.fill: parent
        //color: "black" // this is what I want for the day we present
        color: dragTarget.containsDrag ? "grey" : (colorKey == "any" ? "white" : dragTarget.colorKey)
    }
}
