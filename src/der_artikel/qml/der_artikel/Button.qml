import QtQuick 2.0

Rectangle {
    id:btn_root

    property alias buttonText:label.text
    property alias textPxSize:label.font.pixelSize
    property bool isDown:false
    property color btnColor: "#dadef1"
    property alias text_anchors: label.anchors
    signal activated();


    width: label.width+12
    height: label.height+8
    radius: 0
    gradient: Gradient {
        GradientStop {
            position: 0
            color: "#8d8b8b"
        }

        GradientStop {
            position: 0.2
            color: mousearea1.pressed || isDown ? "#8d8b8b":btnColor
        }

        GradientStop {
            position: 0.8
            color: btnColor
        }

        GradientStop {
            position: 1
            color: "#8d8b8b"
        }
    }

    border.width: mousearea1.pressed | isDown ? 1:0

    Text {
        id: label
        x:2
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Button")
        font.family: custom_command.name
        font.pixelSize: 16
    }

    MouseArea {
        id: mousearea1
        anchors.fill: parent
        onClicked: {
            btn_root.activated()
        }
    }
}
