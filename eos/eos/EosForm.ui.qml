import QtQuick 2.4
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.11

Item {
    id: item1
    width: 520
    height: 400
    property alias button: button
    property alias textA: textA
    property alias textB: textB
    property alias textP: textP
    property alias labelv3: labelv3
    property alias labelv2: labelv2
    property alias labelv1: labelv1
    property alias button1: button1
    property alias textT: textT

    Label {
        id: label
        x: 59
        text: qsTr("Redlich Kwongova jednadžba stanja")
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 25
        font.pointSize: 11
    }

    Row {
        id: row
        x: 259
        y: 333
        width: 214
        height: 40
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.rightMargin: 15

        Button {
            id: button
            text: qsTr("Reset")
        }

        ToolSeparator {
            id: toolSeparator
        }

        Button {
            id: button1
            text: qsTr("Calculate")
        }
    }

    Label {
        id: label1
        x: 15
        y: 85
        text: qsTr("T =")
    }

    TextField {
        id: textT
        x: 42
        y: 72
        placeholderText: qsTr("")
    }

    Label {
        id: label2
        x: 15
        y: 143
        text: qsTr("P =")
    }

    TextField {
        id: textP
        x: 40
        y: 133
        placeholderText: qsTr("")
    }

    Label {
        id: label3
        x: 15
        y: 206
        text: qsTr("a =")
    }

    TextField {
        id: textA
        x: 40
        y: 196
        placeholderText: qsTr("")
    }

    Label {
        id: label4
        x: 15
        y: 270
        text: qsTr("b =")
    }

    TextField {
        id: textB
        x: 40
        y: 260
        placeholderText: qsTr("")
    }

    Label {
        id: label5
        x: 255
        y: 87
        text: qsTr("v1 =")
    }

    Label {
        id: labelv1
        x: 291
        y: 87
        text: qsTr("")
    }

    Label {
        id: label6
        x: 255
        y: 137
        text: qsTr("v2 =")
    }

    Label {
        id: labelv2
        x: 291
        y: 137
        text: qsTr("")
    }

    Label {
        id: label7
        x: 255
        y: 187
        text: qsTr("v3 =")
    }

    Label {
        id: labelv3
        x: 291
        y: 187
        text: qsTr("")
    }
    states: [
        State {
            name: "state_one_solution"

            PropertyChanges {
                target: label6
                opacity: 0
            }

            PropertyChanges {
                target: labelv2
                opacity: 0
            }

            PropertyChanges {
                target: label7
                opacity: 0
            }

            PropertyChanges {
                target: labelv3
                opacity: 0
            }
        }
    ]
}
