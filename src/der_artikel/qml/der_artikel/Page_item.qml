import QtQuick 2.0
import com.vystosi.qmlcomponents 1.0
Item {
    id: root_item
    width: 80
    height: 80
    property alias label_text:item_label.text
    property alias page_icon:icon.source
    Image {
        id: icon
        width:64
        height: width
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        fillMode: Image.PreserveAspectFit
        source: "qrc:/res/resources/default_thema.png"
    }

    Text {
        id: item_label
        text: qsTr("Unkown Page")
        anchors.top: icon.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.topMargin: 0
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        smooth:true
        color:"white"
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        onClicked: {
            manager.current_page=model.src_page_id
        }
    }
}
