//******************************************************************************
/*! \file 
 *
 *  \author Vikas Pachdha
 *
 *  \copyright Copyright (C) 2014 Vikas Pachdha, Mohita Gandotra.
 * Contact: http://www.vikaspachdha.com
 *
 * This file is part of the application der_artikel.
 *
 *  \copyright GNU Lesser General Public License Usage
 * This file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *  \copyright GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 ******************************************************************************/
import QtQuick 2.1
import com.vystosi.qmlcomponents 1.0

Page {
    id:root_item
    page_id:Manager.THEMA_PAGE

    GridView {
        id: grid_view
        anchors.fill: parent
        anchors.margins: 8
        model: themaModel
        cellHeight: settings.thema_item_width + 14
        cellWidth: settings.thema_item_height + 14
        delegate: Thema_item {
            height: settings.thema_item_width
            width: settings.thema_item_height
            property string last_played_str: Qt.formatDateTime(last_played,"dd.MM.yy - hh:mm")
            title:display_name
            translation: tr_name
            author_name: author
            last_played_text:(last_played_str === "" ? "...": last_played_str +qsTr(" hrs")) +settings.i18n_empty_string
            icon_url: "image://rootImageProvider/"+display_name
            count:word_count
            thema_selected: selected
            current_state:thema_state
            onThemaClicked: {
                if(flipped) {
                    flipped = !flipped
                }else {
                    themaModel.clearSelection()
                    thema_object.selected = !thema_object.selected
                }
            }
            onThemaLongClicked: {
                thema_object.selected = true;
            }
        }
    }

    Scroll_bar {
        id: scroll_bar
        anchors {
            top:grid_view.top
            topMargin: 6
            bottom:grid_view.bottom
            bottomMargin: 6
            right: root_item.right
            rightMargin: 4
        }
        orientation: Qt.Vertical
        position: grid_view.visibleArea.yPosition
        pageSize: grid_view.visibleArea.heightRatio

        opacity: grid_view.movingVertically ? 0.7 : 0
    }
}
