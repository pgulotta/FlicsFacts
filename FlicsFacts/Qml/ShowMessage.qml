import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1

Popup {
    id: messagePopupId
    modal: true
    background: backgroundRectId
    x: isPortraitMode ? windowWidth * .1 : windowWidth * .35
    y: windowHeight * .15
    width: isPortraitMode ? windowWidth * .8 : windowWidth * .4
    height: isPortraitMode ? windowHeight * .15 : windowHeight * .25
    readonly property int closeTimerDuration: 1000
    readonly property int gridControlsWidth: messagePopupId.availableWidth * .9
    property string titleText: MovieViewManager.appName
    property string messageText: ""

    function show(title, message) {
        titleText = title
        messageText = message
        messagePopupId.open()
        closeTimerId.restart()
    }

    Timer {
        id: closeTimerId
        interval: closeTimerDuration
        running: false
        onTriggered: {
            messagePopupId.close()
        }
    }
    Rectangle {
        id: backgroundRectId
        color: Material.background
    }

    MouseArea {
        height: parent.height
        width: parent.width
        onClicked: messagePopupId.close()
    }
    TitleLabel {
        id: titleTextId
        text: titleText
        anchors.top: messagePopupId.top
        anchors.topMargin: spacingIndent
    }

    TextLabel {
        id: messageTextId
        text: messageText
        width: messagePopupId.width * .8
        height: messagePopupId.height * .8
        anchors.top: titleTextId.bottom
        anchors.topMargin: spacingIndent
        anchors.left: titleTextId.left
        anchors.leftMargin: spacingIndent
        anchors.right: messagePopupId.right
        anchors.rightMargin: spacingIndent
    }
}
