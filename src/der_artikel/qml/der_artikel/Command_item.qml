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

Item {
    id: root_item
    width: 52
    height: 52
    property alias cmd_text:item_label.text
    property string icon_name:"command"

    signal commandActivated

    Image {
        id: icon
        height: width
        width:32
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        source: mouse_area.pressed ? "qrc:/res/resources/" + icon_name +"_active.png" : root_item.enabled ? "qrc:/res/resources/" + icon_name +".png" : "qrc:/res/resources/" + icon_name +"_disabled.png"
    }

    Item {
        id: lableParent
        anchors.top: icon.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        Text {
            id: item_label
            anchors.baseline: parent.verticalCenter
            anchors.baselineOffset: 4
            anchors.right: parent.right
            anchors.left: parent.left
            color:color_palette.color_font_01
            font.family: regular_font.name
            font.pixelSize: settings.cmd_text_size
            horizontalAlignment: Text.AlignHCenter
        }
    }



    MouseArea {
        id: mouse_area
        anchors.fill: parent
        onClicked: {
            root_item.commandActivated()
            clickSnd.play()
        }
    }
}
