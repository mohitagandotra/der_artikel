import QtQuick 2.1

Flickable {
    anchors.fill: parent
    anchors.margins: 4
    Text {
        id: content_text
        anchors.fill: parent
        text:"1.Time alllocation for a gameplay depends on the game level selected.\
<table border=\"1\">
<tr>
<td><b>Game level</b></td>
<td><b>Time (secs)</b></td>
</tr>
<tr>
<td>Easy</td>
<td>words-count x 4</td>
</tr>
<tr>
<td>Moderate</td>
<td>words-count x 3</td>
</tr>
<tr>
<td>Strict</td>
<td>words-count x 2</td>
</tr>
</table> \
<br><br>2. Correct article gains experience points.<br><br>3. Incorrect article shall \
result into experience points deduction. Magnitude of the deduction shall depend on the \
game level selected for gamplay.<br><br>4. Unplayed words may \
result into experience points deduction. Magnitude of the deduction shall depend on the \
game level selected for gamplay.<br><br>5. Gaining points changes the state of the thema. \
<table border=\"1\">
<tr>
<td><b>Experience</b></td>
<td><b>State</b></td>
</tr>
<tr>
<td>&lt;100</td>
<td>RUSTY</td>
</tr>
<tr>
<td>&gt;100 & &lt;250</td>
<td>SILVER</td>
</tr>
<tr>
<td>&gt;250 & &lt;500</td>
<td>GOLD</td>
</tr>
<tr>
<td>&gt;500</td>
<td>INERT</td>
</tr>
</table> \
<br><br>6. A thema shall lose points if experinece points gain does not occue within 24 hrs \
and the magnitude of points deduction shall depend on the current state of thema.\
<table border=\"1\" bordercolor=\"white\">
<tr>
<td><b>State</b></td>
<td><b>Experience deduction</b></td>
</tr>
<tr>
<td>RUSTY</td>
<td>20</td>
</tr>
<tr>
<td>SILVER</td>
<td>10</td>
</tr>
<tr>
<td>GOLD</td>
<td>5</td>
</tr>
<tr>
<td>INERT</td>
<td>2</td>
</tr>
</table> \
<br>CAUTION: Points are deducted progressively. i.e if your experience points are 500 and \
you did not earn score for the thema for consective 3 days then 12 points shall be deducted."

        wrapMode: Text.WordWrap
        font.family: regular_font.name
        color:color_palette.color_font_01
        font.pixelSize: 16
        textFormat: Text.RichText
    }
    contentHeight: content_text.contentHeight
}
