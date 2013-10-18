import QtQuick 2.0
import QtQuick.Controls 1.0

Rectangle {
    id: msg_bar
    color:"blue"
    property alias title:msg_title.text
    property alias message:msg_text.text
    property alias animation_pause:pause_anim.duration
    property  alias animate: animation.running
    y: -msg_bar.parent.height
    Rectangle {
        id: icon
        height: width
        color:"red"
        anchors {
            left: msg_bar.left
            top: msg_bar.top
            bottom: msg_bar.bottom
        }
    }

    Text {
        id: msg_title
        height: msg_bar.height * 0.40
        anchors {
            left: icon.left
            top: msg_bar.top
            right: msg_bar.right
        }
        font.pointSize: height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        text:"Title"
    }

    Text {
        id: msg_text
        height: msg_bar.height * 0.40
        anchors {
            left: icon.left
            top: msg_title.bottom
            right: msg_bar.right
            bottom: msg_bar.bottom
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        font.pointSize: height/3
        text:"message"
    }

    SequentialAnimation {
        id: animation
        NumberAnimation {
            target: msg_bar
            property: "y"
            to: (msg_bar.parent.height - msg_bar.height)*0.5
            duration: 400
            easing.type: Easing.OutCubic

        }
        PauseAnimation { id: pause_anim; duration: 2000 }
        NumberAnimation {
            target: msg_bar
            property: "y"
            to: msg_bar.parent.height
            duration: 400
            easing.type: Easing.OutCubic

        }
        NumberAnimation {
            target: msg_bar
            property: "y"
            to: -msg_bar.parent.height
            duration: 0
            easing.type: Easing.OutCubic

        }
    }

}
