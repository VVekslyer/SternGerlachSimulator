// Main.qml
import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: mainWindow
    width: 1920/2
    height: 1080
    visible: true
    title: qsTr("Stern-Gerlach Simulator")

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // Top control panel
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#333333"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 15

                // Radio buttons for states
                ButtonGroup {
                    id: stateGroup
                    buttons: row.children
                }

                Row {
                    id: row
                    spacing: 10

                    RadioButton {
                        text: "+Z"
                        checked: true
                        onCheckedChanged: if (checked) console.debug("Selected: +Z")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        text: "-Z"
                        onCheckedChanged: if (checked) console.debug("Selected: -Z")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        text: "+X"
                        onCheckedChanged: if (checked) console.debug("Selected: +X")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        text: "-X"
                        onCheckedChanged: if (checked) console.debug("Selected: -X")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        text: "+Y"
                        onCheckedChanged: if (checked) console.debug("Selected: +Y")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        text: "-Y"
                        onCheckedChanged: if (checked) console.debug("Selected: -Y")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                    RadioButton {
                        id: customButton
                        text: "Custom"
                        onCheckedChanged: if (checked) console.debug("Selected: Custom")
                        contentItem: Text {
                            text: parent.text
                            color: "#CCCCCC"
                            leftPadding: parent.indicator.width + parent.spacing
                        }
                    }
                }

                // Custom angle controls
                RowLayout {
                    visible: customButton.checked
                    spacing: 10

                    Label {
                        text: "θ:"
                        color: "#CCCCCC"
                    }
                    SpinBox {
                        id: thetaSpinBox
                        from: 0
                        to: 314
                        stepSize: 1
                        value: 0
                        onValueChanged: console.debug("θ =", value/100 * Math.PI)
                        
                        TextMetrics {
                            id: textMetrics
                            text: thetaSpinBox.value/100
                        }
                    }

                    Label {
                        text: "ϕ:"
                        color: "#CCCCCC"
                    }
                    SpinBox {
                        id: phiSpinBox
                        from: 0
                        to: 628
                        stepSize: 1
                        value: 0
                        onValueChanged: console.debug("ϕ =", value/100 * Math.PI)
                    }
                }

                RowLayout {
                    spacing: 10

                    Label {
                        text: "How many particles to start?"
                        color: "#CCCCCC"  // Match other text color
                    }
                    SpinBox {
                        id: particleCountSpinBox
                        from: 1
                        to: 1000
                        stepSize: 1
                        value: 100  // Default value
                        onValueChanged: console.debug("Particle count:", value)
                    }
                }
            }
        }

        // Tiles component
        Loader {
            id: tilesLoader
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: "tiles/tiles.qml"
        }
    }

    // Property to expose selected state to tiles.qml
    property string selectedState: {
        if (customButton.checked) {
            return `custom,${thetaSpinBox.value/100 * Math.PI},${phiSpinBox.value/100 * Math.PI}`;
        }
        return stateGroup.checkedButton ? stateGroup.checkedButton.text : "+Z";
    }
}
