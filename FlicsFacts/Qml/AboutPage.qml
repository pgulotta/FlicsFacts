import QtQuick 2.7
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.1

Page {
    id: aboutPageId

    signal backButtonSelected
    readonly property int gridControlsWidth: aboutPageId.availableWidth * .9

    header: ToolBar {
        id: aboutToolBarId
        Material.elevation: 4
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
            onClicked: backButtonSelected()
        }

        Label {
            text: MovieViewManager.appName
            color: "white"
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }

    TitleLabel {
        id: aboutTitleId
        text: qsTr("About")
        width: parent.width * 0.80
        anchors.left: parent.left
        anchors.leftMargin: spacingIndent
        anchors.top: parent.top
        anchors.topMargin: spacingIndent
    }
    TextLabel {
        id: aboutDescriptionId
        text: qsTr("FlicsFacts v" + MovieViewManager.appVersion
                   + " from 26Apps provides movie information for the selected Flic. Visit "
                   + "<a href=\"" + websiteLink + "\">website.</a>")
        onLinkActivated: Qt.openUrlExternally(websiteLink)
        anchors.top: aboutTitleId.bottom
        anchors.topMargin: textMargin
        anchors.left: parent.left
        anchors.leftMargin: spacingIndent
        anchors.right: parent.right
        anchors.rightMargin: spacingIndent
    }

    Grid {
        id: aboutGridId
        width: parent.width * 0.80
        anchors.top: aboutDescriptionId.bottom
        anchors.topMargin: textMargin
        anchors.left: parent.left
        anchors.leftMargin: spacingIndent
        anchors.right: parent.right
        anchors.rightMargin: spacingIndent
        columns: 1
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
    onBackButtonSelected: StackView.view.pop()
}
