import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

ToolBar {
    property string pageTitle: MovieViewManager.appName

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
        onClicked: onBackSelected()
    }
    Label {
        text:pageTitle
        color: "white"
        font.pixelSize: fontSizeXLarge
        anchors.centerIn: parent
    }

}
