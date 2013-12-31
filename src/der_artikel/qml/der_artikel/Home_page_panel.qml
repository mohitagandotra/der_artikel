import QtQuick 2.0
import com.vystosi.qmlcomponents 1.0

Page_panel
{
    page_id:Manager.HOME_PAGE
    Label {
        id: home_lbl
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 2
        text_anchors.horizontalCenter: home_lbl.horizontalCenter
        lblText: qsTr("Home")
    }

    Column {
        id: home_btn_col
        anchors.top: home_lbl.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 4
        spacing: 4

        Button {
            id:rate_btn
            width:parent.width
            text_anchors.horizontalCenter: rate_btn.horizontalCenter
            buttonText: qsTr("Rate App")
            onActivated: showMessage(qsTr("Sorry"), qsTr("This feature is not available"),1200)
        }
    }

}
