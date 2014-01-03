import QtQuick 2.0
import com.vystosi.qmlcomponents 1.0

Page {
    id:root
    page_id:Manager.STATS_PAGE

    property int thema_count:0
    property int inert_thema_count:0
    property int golden_thema_count:0
    property int silver_thema_count:0
    property int rusty_thema_count:0
    property string best_thema_name:""

    Flickable {
        anchors.fill: parent
        contentHeight: 6 * 32 + 12
        Repeater {
        model:6
        delegate: Result_item {
            height: 30
            width:parent.width - 12
            x:6
            y: (index*32) + 6
            left_text: {
                switch(index) {
                    case 0:return qsTr("Thema count")
                    case 1:return qsTr("Best Thema")
                    case 2:return qsTr("Inert themas");
                    case 3:return qsTr("Golden themas")
                    case 4:return qsTr("Silver themas")
                    case 5:return qsTr("Rusty themas")
                }
            }
            right_text: {
                switch(index) {
                    case 0:return thema_count
                    case 1:return best_thema_name
                    case 2:return inert_thema_count
                    case 3:return golden_thema_count
                    case 4:return silver_thema_count
                    case 5:return rusty_thema_count
                }
            }
            left_text_color: color_palette.color_font_01
            right_text_color: color_palette.color_font_01
            center_text: " : "
        }
    }
    }
}
