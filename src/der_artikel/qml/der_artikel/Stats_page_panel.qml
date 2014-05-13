import QtQuick 2.1
import com.vystosi.qmlcomponents 1.0

Page_panel
{
    page_id: Manager.STATS_PAGE

    Label {
        id: stats_lbl
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 2
        text_anchors.horizontalCenter: stats_lbl.horizontalCenter
        lblText: qsTr("Lets Brag!") + settings.i18n_empty_string
    }

    Column {
        id: help_btn_col
        anchors.top: stats_lbl.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 4
        spacing: 6

        Button {
            id:share_btn
            width:parent.width
            text_anchors.horizontalCenter: share_btn.horizontalCenter
            buttonText: qsTr("Share") + settings.i18n_empty_string
            onActivated: showMessage(qsTr("Sorry"), qsTr("This feature is not available."),1200)
        }
    }
}
