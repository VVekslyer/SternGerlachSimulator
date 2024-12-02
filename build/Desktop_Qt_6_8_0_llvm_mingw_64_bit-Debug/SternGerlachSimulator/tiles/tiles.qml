// tiles/tiles.qml
import QtQuick 2.15

Rectangle {
    id: root
    width: 1280
    height: 720
    color: "black"

    // Add counter for unique IDs
    property int nextId: 0

    // Add locked property to grid cells
    property var gridArray: {
        let arr = [];
        for (let i = 0; i < 12; i++) {
            arr[i] = [];
            for (let j = 0; j < 12; j++) {
                arr[i][j] = { type: ".", id: -1, locked: false };
            }
        }
        return arr;
    }

    // Add pattern checking function
    function checkPatterns(gridX, gridY) {
        // Check all possible patterns
        const patterns = [
            [{x:0, y:0, type:"X"}, {x:2, y:0, type:"-"}, {x:2, y:1, type:"|"}],
            [{x:0, y:0, type:"X"}, {x:2, y:0, type:"|"}, {x:2, y:1, type:"-"}],
            [{x:0, y:0, type:"Z"}, {x:2, y:0, type:"-"}, {x:2, y:1, type:"|"}],
            [{x:0, y:0, type:"Z"}, {x:2, y:0, type:"|"}, {x:2, y:1, type:"-"}]
        ];

        // Check each pattern starting from the given position
        for (let pattern of patterns) {
            let matches = true;
            for (let cell of pattern) {
                let checkX = gridX + cell.x;
                let checkY = gridY + cell.y;
                
                if (checkX >= 12 || checkY >= 12 || 
                    gridArray[checkY][checkX].type !== cell.type) {
                    matches = false;
                    break;
                }
            }

            if (matches) {
                // Lock the matching cells
                for (let cell of pattern) {
                    let lockX = gridX + cell.x;
                    let lockY = gridY + cell.y;
                    gridArray[lockY][lockX].locked = true;
                }
                console.debug("Pattern found and locked!");
                return true;
            }
        }
        return false;
    }

    // Update printGrid function to handle new structure
    function printGrid() {
        console.debug("\nGrid State:");
        for (let i = 0; i < 12; i++) {
            let row = "";
            for (let j = 0; j < 12; j++) {
                row += gridArray[i][j].type + " ";
            }
            console.debug(row);
        }
        console.debug("-------------------");
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
        opacity: 0.5
        columns: 12  // Increased since tiles are smaller

        Repeater {
            model: 144   // Increased grid capacity
            delegate: DropTile {
                id: dropTile
                colorKey: "any"
                property bool isDragActive: false
                property var lastDragSource: null
                property var lastDragId: null
                
                onEntered: {
                    isDragActive = true;
                    lastDragSource = drag.source;
                    // Assign new ID if from sourceColumn, otherwise keep existing
                    let isFromSourceColumn = lastDragSource.parent.toString().includes("sourceColumn");
                    lastDragId = isFromSourceColumn ? root.nextId++ : lastDragSource.dragId;
                    console.debug("Drag entered:", index, "with ID:", lastDragId);
                }
                
                onExited: {
                    console.debug("Exit detected, isDragActive:", isDragActive);
                    if (isDragActive) {
                        let type = ".";
                        if (lastDragSource) {
                            if (drag.source.Drag.keys.indexOf("red") >= 0) type = "X";
                            else if (drag.source.Drag.keys.indexOf("blue") >= 0) type = "Z";
                            else if (drag.source.Drag.keys.indexOf("white") >= 0) type = "-";
                            else if (drag.source.Drag.keys.indexOf("wall") >= 0) type = "|";
                            else if (drag.source.Drag.keys.indexOf("output") >= 0) type = "o"; // Changed from "O" to "o"

                            let isFromSourceColumn = lastDragSource.parent.toString().includes("sourceColumn");
                            if (!isFromSourceColumn) {
                                // Clear only the specific instance using ID
                                for (let i = 0; i < 12; i++) {
                                    for (let j = 0; j < 12; j++) {
                                        if (gridArray[i][j].type === type && 
                                            gridArray[i][j].id === lastDragId) {
                                            gridArray[i][j] = { type: ".", id: -1, locked: false };
                                        }
                                    }
                                }
                            }

                            let gridX = Math.floor(index % 12);
                            let gridY = Math.floor(index / 12);
                            
                            // Store both type and ID
                            gridArray[gridY][gridX] = { type: type, id: lastDragId, locked: false };
                            console.debug("Drop completed at:", gridX, gridY, "with ID:", lastDragId);
                            checkPatterns(gridX, gridY);
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

        // Add after other Repeaters
        Repeater {
            model: [0]
            delegate: Output {
                text: "-"  // Customize text as needed
            }
        }
    }
}
