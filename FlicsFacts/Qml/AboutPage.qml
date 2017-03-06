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
        anchors.left: parent.left
        anchors.leftMargin: spacingIndent
        anchors.top: parent.top
        anchors.topMargin: spacingIndent
    }
    Grid {
        id: aboutGridId
        width: aboutPageId.width * .6
        height: aboutPageId.height * .6
        anchors.top: aboutTitleId.bottom
        anchors.topMargin: textMargin
        anchors.left: aboutTitleId.left
        anchors.leftMargin: spacingIndent
        anchors.right: aboutTitleId.right
        anchors.rightMargin: spacingIndent
        columns: 1

        TextLabel {
            width: aboutPageId.availableWidth * .85
            text: qsTr("FlicsFacts v" + MovieViewManager.appVersion
                       + " from 26Apps provides movie information for the selected Flic. Visit "
                       + "<a href=\"" + websiteLink + "\">website.</a>")
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
    onBackButtonSelected: StackView.view.pop()
}
