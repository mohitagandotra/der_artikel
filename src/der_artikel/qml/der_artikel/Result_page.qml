import QtQuick 2.1
import com.vystosi.qmlcomponents 1.0

Page {
    page_id:Manager.RESULT_PAGE
    id: rectangle1

    Custom_label {
        id: title_label
        text: (list_view.count>0 ? qsTr("Mistakes") : qsTr("No Mistakes")) + settings.i18n_empty_string
        font_color:color_palette.color_font_02
        gradient: Gradient {
            GradientStop {position:0.0;color:color_palette.color_btn_02}
            GradientStop {position:0.1;color:color_palette.color_btn_01}
            GradientStop {position:0.9;color:color_palette.color_btn_01}
            GradientStop {position:1.0;color:color_palette.color_btn_02}
        }
        radius:4
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Custom_label {
        id: header_label
        text: qsTr("Incorrect       Correct") + settings.i18n_empty_string
        font_color:color_palette.color_font_02
        visible:list_view.count > 0
        gradient: Gradient {
            GradientStop {position:0.0;color:color_palette.color_btn_02}
            GradientStop {position:0.1;color:color_palette.color_btn_01}
            GradientStop {position:0.9;color:color_palette.color_btn_01}
            GradientStop {position:1.0;color:color_palette.color_btn_02}
        }
        radius:4
        anchors {
            top: title_label.bottom
            topMargin: 2
            left: parent.left
            right: parent.right
        }
    }

    ListView {
        id: list_view

        anchors.top: header_label.bottom
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        spacing:4
        clip:true
        model: currentResult.incorrectWordsModel
        delegate: Result_item {
            //height: 24
            right_text: articleText(article)+" " + word_text
            left_text: word_text
            correct_article: article
        }
    }

    function onResultUpdated()
    {
        title_label.text = currentResult.grade_string
        score_label.text = currentResult.score_string
        mistakes_label.text = currentResult.mistake_string
        unplayed_label.text = currentResult.unplayed_string
    }
}
