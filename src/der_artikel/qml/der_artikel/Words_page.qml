import QtQuick 2.0

Item {
    id: word_page_root

    Flickable
    {
        id: wordsFrame
        anchors.fill: word_page_root
        clip: true
        contentHeight: wordFlow.childrenRect.height
        Flow
        {
            id: wordFlow
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            spacing: 6
        }
    }


    function appendWordToFlow(itemText)
    {
        var WordComponent = Qt.createComponent("Word_item.qml");
        if(WordComponent.status == Component.Ready) {
            var WordItem = WordComponent.createObject(wordFlow);
            WordItem.wordText = itemText;
        }

        return WordItem;
    }
}
