import QtQuick 2.0

Item {
    id:lbl_root

    property alias lblText:label.text
    property alias textPxSize:label.font.pixelSize
    property alias text_anchors: label.anchors

    width: label.width+12
    height: label.height+8
    Text {
        id: label
        x:2
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("Label")
        color:cp_blue.colorf01
        font.pixelSize: 14
    }
}
