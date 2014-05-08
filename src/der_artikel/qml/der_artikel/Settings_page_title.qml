import QtQuick 2.1
import com.vystosi.qmlcomponents 1.0

Title_bar {
    page_id: Manager.SETTINGS_PAGE
    Text {
        id: name
        text: qsTr("Settings") + settings.i18n_empty_string
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.family: title_font.name
        color:color_palette.color_font_01
        font.pixelSize: 42
    }
}
