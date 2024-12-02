// tiles/tiles.qml
import QtQuick 2.15
import SternGerlach 1.0

Rectangle {
    id: root
    width: 1280
    height: 720
    color: "black"

    // Add property to receive state from Main.qml
    property string initialState: Window.window ? Window.window.selectedState : "+Z"
    property int particleCount: tilesLoader.particleCount  // Update this

    SternGerlachSimulator {
        id: simulator
    }

    onInitialStateChanged: {
        console.debug("Initial state changed to:", initialState);
        // Handle state change logic here
    }

    // Add counter for unique IDs
    property int nextId: 0

    // Add locked property to grid cells
    property var gridArray: {
        let arr = [];
        for (let i = 0; i < 16; i++) {  // Changed from 12 to 16
            arr[i] = [];
            for (let j = 0; j < 16; j++) {  // Changed from 12 to 16
                arr[i][j] = { type: ".", id: -1, locked: false };
            }
        }
        return arr;
    }

    // Update printGrid function to handle new structure
    function printGrid() {
        console.debug("\nGrid State:");
        for (let i = 0; i < 16; i++) {  // Changed from 12 to 16
            let row = "";
            for (let j = 0; j < 16; j++) {  // Changed from 12 to 16
                row += gridArray[i][j].type + " ";
            }
            console.debug(row);
        }
        console.debug("-----------------------");
    }

    // After grid updates, check pattern
    function checkPattern() {
        let gridData = [];
        for (let i = 0; i < 16; i++) {
            for (let j = 0; j < 16; j++) {
                gridData.push(gridArray[i][j].type);
            }
        }
        simulator.analyzeGrid(gridData, initialState, particleCount);
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
        columns: 16  // Changed from 12 to 16

        Repeater {
            model: 256  // Changed from 144 (16x16 = 256)
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
                            else if (drag.source.Drag.keys.indexOf("orange") >= 0) type = "Y"; // Add this line
                            else if (drag.source.Drag.keys.indexOf("white") >= 0) type = "-";
                            else if (drag.source.Drag.keys.indexOf("wall") >= 0) type = "|";
                            else if (drag.source.Drag.keys.indexOf("output") >= 0) type = "o"; // Changed from "O" to "o"

                            let isFromSourceColumn = lastDragSource.parent.toString().includes("sourceColumn");
                            if (!isFromSourceColumn) {
                                // Clear only the specific instance using ID
                                for (let i = 0; i < 16; i++) {
                                    for (let j = 0; j < 16; j++) {
                                        if (gridArray[i][j].type === type && 
                                            gridArray[i][j].id === lastDragId) {
                                            gridArray[i][j] = { type: ".", id: -1, locked: false };
                                        }
                                    }
                                }
                            }

                            let gridX = Math.floor(index % 16);
                            let gridY = Math.floor(index / 16);
                            
                            // Store both type and ID
                            gridArray[gridY][gridX] = { type: type, id: lastDragId, locked: false };
                            console.debug("Drop completed at:", gridX, gridY, "with ID:", lastDragId);
                            printGrid();
                        }
                    }
                    isDragActive = false;
                    lastDragSource = null;
                    checkPattern();
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
                colorKey: "orange"
                axisLabel: "y"
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
            model: [0, 1, 2]
            delegate: ConnectionLine { }
        }

        Repeater {
            model: [0, 1, 2]
            delegate: Wall { }
        }

        // Add after other Repeaters
        Repeater {
            model: [0]
            delegate: Output {
                text: "out"  // Customize text as needed
            }
        }
    }
}
