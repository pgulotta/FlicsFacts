import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1

Popup {
    id: aboutPopupId
    modal: true
    background: backgroundRectId
    x: windowWidth * .15
    y: windowHeight * .05
    width: windowWidth * .7
    height: isPortraitMode ? windowHeight * .55 : windowHeight * .75
    readonly property int gridControlsWidth: aboutPopupId.availableWidth * .9

    Rectangle {
        id: backgroundRectId
        color: Material.background
    }

    MouseArea {
        height: parent.height
        width: parent.width
        onClicked: aboutPopupId.close()
    }

    TitleLabel {
        id: aboutId
        text: qsTr("About")
        anchors.top: aboutPopupId.top
        anchors.topMargin: spacingIndent
    }
    Grid {
        id: aboutGridId
        width: aboutPopupId.width * .6
        height: aboutPopupId.height * .6
        anchors.top: aboutId.bottom
        anchors.topMargin: textMargin
        anchors.left: aboutId.left
        anchors.leftMargin: spacingIndent
        anchors.right: aboutId.right
        anchors.rightMargin: spacingIndent
        columns: 1

        TextLabel {
            width: aboutPopupId.availableWidth * .85
            text: qsTr("FlicsFacts v" + MovieViewManager.appVersion
                       + " from 26Apps provides movie information for the selected Flic. Visit "+
                       "<a href=\"" + websiteLink + "\">website.</a>")
            onLinkActivated: Qt.openUrlExternally(websiteLink)
        }

        Label {
            text: " "
        }
        HeadingLabel {
            text: qsTr("Credits")
        }
        TextLabel {
            width: gridControlsWidth
            text: "<a href=\"" + tmdbLink + "\">TMDB API</a>"
                  + " This product uses the TMDb API but is not endorsed or certified by TMDb."
            onLinkActivated: Qt.openUrlExternally(tmdbLink)
        }
        TextLabel {
            width: gridControlsWidth
            text: "<a href=\"" + qtLink + "\">Qt Company</a>"
                  + " software is used to develop FlicFacts."
            onLinkActivated: Qt.openUrlExternally(qtLink)
        }
        Label {
            text: " "
        }
        HeadingLabel {
            text: qsTr("Privacy")
        }
        TextLabel {
            width: gridControlsWidth
            text: qsTr("FlicFacts does not request or share any personal information with third parties.It does not request or know the user's physical location.")
        }
    }
}
