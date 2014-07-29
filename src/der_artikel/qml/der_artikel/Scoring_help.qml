import QtQuick 2.1

Flickable {
    anchors.fill: parent
    anchors.margins: 4
    contentHeight: content_text.contentHeight
    Text {
        id: content_text
        anchors.fill: parent
        text:qsTr("<p>Scoring:<table border=\"1\"> \
<tr> \
<td><b>Game level</b></td> \
<td>Correct article</td> \
<td>Incorrect article</td> \
<td>Unplayed word</td> \
</tr> \
<tr> \
<td>EASY</td> \
<td>1</td> \
<td>-0.5</td> \
<td>-0.25</td> \
</tr> \
<tr> \
<td>Moderate</td> \
<td>1</td> \
<td>-0.5</td> \
<td>0</td> \
</tr> \
<tr> \
<td>Expert</td> \
<td>1</td> \
<td>0</td> \
<td>0</td> \
</tr> \
</table> \
</p><p>Grading:<table border=\"1\"> \
<tr> \
<td><b>SCORE</b></td> \
<td>Grade</td> \
</tr> \
<tr> \
<td>&gt;94.99</td> \
<td>A+</td> \
</tr> \
<tr> \
<td>&gt;89.99</td> \
<td>A</td> \
</tr> \
<tr> \
<td>&gt;79.99</td> \
<td>B+</td> \
</tr> \
<tr> \
<td>&gt;69.99</td> \
<td>B</td> \
</tr> \
<tr> \
<td>&gt;59.99</td> \
<td>C</td> \
</tr> \
<tr> \
<td>&gt;49.99</td> \
<td>D</td> \
</tr> \
<tr> \
<td>&lt;49.99</td> \
<td>E</td> \
</tr> \
</table> \
</p><p>Experience:<table border=\"1\"> \
<tr> \
<td><b>Grade</b></td> \
<td>Easy</td> \
<td>Moderate</td> \
<td>Expert</td> \
</tr> \
<tr> \
<td>A+</td> \
<td>50</td> \
<td>80</td> \
<td>100</td> \
</tr> \
<tr> \
<td>A</td> \
<td>40</td> \
<td>60</td> \
<td>80</td> \
</tr> \
<tr> \
<td>B+</td> \
<td>30</td> \
<td>50</td> \
<td>70</td> \
</tr> \
<tr> \
<td>B</td> \
<td>20</td> \
<td>40</td> \
<td>60</td> \
</tr> \
<tr> \
<td>C</td> \
<td>10</td> \
<td>30</td> \
<td>50</td> \
</tr> \
<tr> \
<td>D</td> \
<td>5</td> \
<td>20</td> \
<td>40</td> \
</tr> \
<tr> \
<td>E</td> \
<td>0</td> \
<td>-10</td> \
<td>-20</td> \
</tr> \
</table></p>") + settings.i18n_empty_string

        textFormat: Text.RichText
        wrapMode: Text.WordWrap
        font.family: regular_font.name
        color:color_palette.color_font_01
        font.pixelSize: normalTextSize
    }
}
