import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

ToolBar {
    id: movieSearchToolBarId

    Component.onCompleted: {
        titleRequestId.forceActiveFocus()
        Qt.inputMethod.hide()
    }

    ToolButton {
        id: backToolButtonId
        visible: true
        anchors.left: parent.left
        anchors.leftMargin: textBorderWidth
        contentItem: Image {
            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:/Images/back.png"
        }
        onClicked: rootId.onBackSelected()
    }
    Rectangle {
        id: searchTextRectId
        width: .5 * windowWidth
        radius: 4
        anchors.left: backToolButtonId.right
        anchors.leftMargin: textBorderWidth
        anchors.top: parent.top
        anchors.topMargin: textMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: textMargin
        border.width: textBorderWidth
        border.color: Material.accent
        TextField {
            id: titleRequestId
            height: tabHeight + 12
            focus: true
            placeholderText: qsTr("Movie Title")
            color: Material.primary
            font.pointSize: fontSizeSmall
            verticalAlignment: Text.AlignVCenter
            anchors.left: searchTextRectId.left
            anchors.leftMargin: textMargin
            anchors.right: searchTextRectId.right
            anchors.top: searchTextRectId.top
            anchors.topMargin: textBorderWidth
            Keys.onReturnPressed: {
                Qt.inputMethod.hide()
                processSearchRequest()
            }
            onFocusChanged: Qt.inputMethod.hide()
        }
    }
    ToolButton {
        id: searchButtonId
        anchors.left: searchTextRectId.right
        anchors.leftMargin: textBorderWidth
        contentItem: Image {
            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:/Images/search.png"
        }
        onClicked: {
            processSearchRequest()
            onFocusChanged: Qt.inputMethod.hide()
        }
    }
    ToolButton {
        id: removeButtonId
        anchors.left: searchButtonId.right
        visible: movieSearchResponses.count !== 0
        contentItem: Image {
            fillMode: Image.Pad
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:/Images/remove.png"
        }
        onClicked: {
            MovieViewManager.removeSelectedMovie(movieSearchPageId.movieIndex)
            onFocusChanged: Qt.inputMethod.hide()
        }
    }

    function processSearchRequest() {
        MovieViewManager.findFlicSelected(titleRequestId.text)
        titleRequestId.text = ""
    }
}
