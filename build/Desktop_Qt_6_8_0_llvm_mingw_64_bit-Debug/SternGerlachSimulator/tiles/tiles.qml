// tiles/tiles.qml
import QtQuick 2.15

Rectangle {
    id: root
    width: 1280
    height: 720
    color: "black"

    // Grid tracking array
    property var gridArray: {
        let arr = [];
        for (let i = 0; i < 12; i++) {
            arr[i] = [];
            for (let j = 0; j < 12; j++) {
                arr[i][j] = ".";
            }
        }
        return arr;
    }

    // Print function
    function printGrid() {
        console.debug("\nGrid State:");
        for (let i = 0; i < 12; i++) {
            let row = "";
            for (let j = 0; j < 12; j++) {
                row += gridArray[i][j] + " ";
            }
            console.debug(row);
        }
        console.debug("---------------");
    }

    Grid {
        id: destination
        anchors {
            left: sourceColumn.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 5
        }
        width: parent.width - sourceColumn.width - 10
        height: parent.height - 10
        opacity: 0.5 // why was this even here?
        columns: 12  // Increased since tiles are smaller

        Repeater {
            model: 144   // Increased grid capacity
            delegate: DropTile {
                id: dropTile
                colorKey: "any"
                property bool isDragActive: false
                property var lastDragSource: null
                
                onEntered: {
                    isDragActive = true;
                    lastDragSource = drag.source;
                    console.debug("Drag entered:", index);
                }
                
                onExited: {
                    console.debug("Exit detected, isDragActive:", isDragActive);
                    if (isDragActive) {
                        console.debug("lastDragSource:", lastDragSource);
                        console.debug("lastDragSource parent:", lastDragSource ? lastDragSource.parent : null);
                        console.debug("dropTile:", dropTile);

                        let type = ".";
                        if (lastDragSource) {
                            if (drag.source.Drag.keys.indexOf("red") >= 0) type = "X";
                            else if (drag.source.Drag.keys.indexOf("blue") >= 0) type = "Z";
                            else if (drag.source.Drag.keys.indexOf("white") >= 0) type = "-";
                            else if (drag.source.Drag.keys.indexOf("wall") >= 0) type = "|";

                            // Only clear previous position if dragging from grid, not sourceColumn
                            let isFromSourceColumn = lastDragSource.parent.toString().includes("sourceColumn");
                            if (!isFromSourceColumn) {
                                for (let i = 0; i < 12; i++) {
                                    for (let j = 0; j < 12; j++) {
                                        if (gridArray[i][j] === type) {
                                            gridArray[i][j] = ".";
                                        }
                                    }
                                }
                            }

                            let gridX = Math.floor(index % 12);
                            let gridY = Math.floor(index / 12);
                            
                            gridArray[gridY][gridX] = type;
                            console.debug("Drop completed at:", gridX, gridY);
                            printGrid();
                        }
                    }
                    isDragActive = false;
                    lastDragSource = null;
                }
            }
        }
    }

    Column {
        id: sourceColumn

        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: 5
        }
        width: 72  // Was 96
        spacing: -4

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile {
                colorKey: "red"
                axisLabel: "x"
            }
        }

        Repeater {
            model: [0, 1, 2]
            delegate: DragTile {
                colorKey: "blue"
                axisLabel: "z"
            }
        }

        Repeater {
            model: [0, 1, 2, 3, 4, 5, 6]
            delegate: ConnectionLine { }
        }

        Repeater {
            model: [0, 1, 2, 3]
            delegate: Wall { }
        }
    }
}
