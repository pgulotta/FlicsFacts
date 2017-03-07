import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Page {
    id: mainPageId

    signal menuSelected(string contextId, bool isClosed)
    signal buttonSelected(string contextId, int modelIndex)

    header: ToolBar {
        id: mainToolBarId
        Material.elevation: 4
        ToolButton {
            id: mainToolButtonId
            visible: true
            anchors.left: parent.left
            anchors.leftMargin: textBorderWidth
            contentItem: Image {
                fillMode: Image.Pad
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: "qrc:/Images/menu.png"
            }
            onClicked: mainDrawerId.open()
        }

        Label {
            text: MovieViewManager.appName
            color: "white"
            font.pixelSize: fontSizeXLarge
            anchors.centerIn: parent
        }
    }

    Image {
        id: mainPaneImageId
        width: isPortraitMode ? windowWidth * .65 : windowHeight * .60
        height: width
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        source: "qrc:/Images/splash.png"
    }
    TextLabel {
        text: MovieViewManager.appNameVersion
        anchors.bottomMargin: spacingIndent
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        horizontalAlignment: "AlignHCenter"
    }
}
