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

Title_bar {

    page_id: Manager.THEMA_PAGE

    Image {
        id:coin
        source: getTexture()
        visible: themaModel.selection_state === ThemaModel.SINGLE_SELECTION ? true:false
        anchors.left: parent.left
        anchors.top:parent.top
        anchors.bottom:parent.bottom
        anchors.leftMargin: 2
        width:height
        fillMode: Image.PreserveAspectFit
        smooth:true
        transform: Rotation {
            id: rotation
            origin.x: coin.width/2
            origin.y: coin.height/2
            axis.x: 0; axis.y: 1; axis.z: 0
            angle: 0
            Behavior on angle {
                NumberAnimation{
                    duration: rotation.angle===0?800:0
                    onRunningChanged: {
                        if(running === false) {
                           rotation.angle = 0
                        }
                    }
                }
            }
        }

        Timer {
            interval: 4000
            running: true
            repeat: true
            onTriggered: {
                rotation.angle = 360
            }
        }
    }

    Text {
        id: thema_name
        text:{
            switch(themaModel.selection_state) {
            case ThemaModel.MULTIPLE_SELECTION:
                return qsTr("Multi Mode - Coming soon") + settings.i18n_empty_string;
            case ThemaModel.SINGLE_SELECTION:
                return themaModel.selected_thema.name;
            default:
                return qsTr("Select Thema") + settings.i18n_empty_string;
            }
        }

        anchors {
            bottom: parent.verticalCenter
            left:parent.left
            right:parent.right
            rightMargin: Math.max(coin.width,heading_1_right.width+heading_1_right.anchors.rightMargin)
            leftMargin: Math.max(coin.width,heading_1_right.width+heading_1_right.anchors.rightMargin)
        }
        color: color_palette.color_font_01
        font.family: title_font.name
        font.pixelSize: settings.heading_1_size
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        elide: Text.ElideRight
    }

    Text {
        id: tr_name
        text:themaModel.selection_state===ThemaModel.SINGLE_SELECTION?themaModel.selected_thema.tr_name:"";

        anchors {
            top: parent.verticalCenter
            left:parent.left
            right:parent.right
            rightMargin: Math.max(coin.width,heading_1_right.width+heading_1_right.anchors.rightMargin)
            leftMargin: Math.max(coin.width,heading_1_right.width+heading_1_right.anchors.rightMargin)
        }
        color: color_palette.color_font_01
        font.family: title_font.name
        font.pixelSize: settings.heading_1_size
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:  Text.AlignVCenter
        elide: Text.ElideRight
    }

    Text {
        id: heading_1_right
        text:themaModel.selection_state===ThemaModel.SINGLE_SELECTION ?
                 qsTr("Experience : ") + settings.i18n_empty_string+themaModel.selected_thema.experience:""
        anchors {
            top:parent.top
            right:parent.right
            rightMargin:6
            topMargin: 6
        }
        color: color_palette.color_font_01
        font.family: title_font.name
        font.pixelSize: settings.heading_1_size
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignLeft
        verticalAlignment:  Text.AlignVCenter
    }

    Text {
        id: heading_2_right
        text: {
            if(themaModel.selection_state===ThemaModel.SINGLE_SELECTION) {
                var currentState = themaModel.selected_thema.state
                var state_str = qsTr("State - ") + settings.i18n_empty_string
                switch(currentState) {
                case Thema.INERT:
                    state_str += qsTr("Inert") + settings.i18n_empty_string
                    break
                case Thema.GOLD:
                    state_str += qsTr("Gold") + settings.i18n_empty_string
                    break;
                case Thema.SILVER:
                    state_str += qsTr("Silver") + settings.i18n_empty_string
                    break;
                default:
                    state_str += qsTr("Rusty") + settings.i18n_empty_string
                    break;
                }
            } else {
                return ""
            }
            return state_str
        }

        anchors {
            bottom:parent.bottom
            bottomMargin: 6
            right:parent.right
            rightMargin:6
        }
        color: color_palette.color_font_01
        font.family: title_font.name
        font.pixelSize: settings.heading_2_size
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignLeft
        verticalAlignment:  Text.AlignTop
    }

    function getTexture()
    {
        var image_url = ""
        if(themaModel.selection_state===ThemaModel.SINGLE_SELECTION) {
            switch(themaModel.selected_thema.state) {
            case Thema.RUSTY:
                image_url = "qrc:/res/resources/50_cent_rusty.png";
                break;
            case Thema.SILVER:
                image_url = "qrc:/res/resources/50_cent_silver.png";
                break;
            case Thema.GOLD:
                image_url = "qrc:/res/resources/50_cent_gold.png";
                break;
            case Thema.INERT:
                image_url = "qrc:/res/resources/50_cent_inert.png";
                break;
            }
        }
        return image_url
    }
}
