/*
 *  Stern-Gerlach Simulator Project
 *  
 *  Copyright © 2024 Vitaliy Vekslyer and Robert Truong
 *
 *  tiles/DropTile.qml
 */

import QtQuick 2.15

DropArea { // Where SG devices and connections are dropped onto
    id: dragTarget

    property string colorKey

    width: 44
    height: 36
    keys: colorKey == "any" ? ["red", "blue", "orange", "white", "line", "wall", "output"] : [ colorKey ]

    Rectangle {
        id: dropRectangle

        anchors.fill: parent
        color: dragTarget.containsDrag ? "grey" : (colorKey == "any" ? "white" : dragTarget.colorKey)
        opacity: 0.1 // clear out the grey color
    }
}
