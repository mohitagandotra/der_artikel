import QtQuick 2.0

Rectangle {

    property alias text:label_text.text
    property alias font_size:label_text.font.pixelSize
    property alias font_color:label_text.color
    property alias text_horizontal_alignment: label_text.horizontalAlignment
    property alias text_vertical_alignment: label_text.verticalAlignment

    height: childrenRect.height+4
    color: "#ABB5DA"

    Text {
        y:2
        id: label_text
        font.pixelSize: 18
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: Text.AlignHCenter
    }
}