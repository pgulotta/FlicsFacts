import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

ApplicationWindow {
    readonly property bool isPortraitMode: Screen.height > Screen.width
    readonly property bool isAndroidPlatform: Qt.platform.os === "android"
    readonly property int gridColumnCount: isPortraitMode ? 2 : 4
    readonly property int windowWidth: isAndroidPlatform ? Screen.width : Screen.width / 3
    readonly property int windowHeight: isAndroidPlatform ? Screen.height : Screen.height / 3
    readonly property int tabHeight: 30
    readonly property int columnItemIndent: 100
    readonly property int textBorderWidth: 2
    readonly property int textMargin: 8
    readonly property int spacingIndent: 20
    readonly property int fontSizeLarge: 18
    readonly property int fontSizeMedium: 16
    readonly property int fontSizeSmall: 14
    readonly property string tmdbLink: "http://www.themoviedb.org/"
    readonly property string qtLink: "http://www.qt.io/"
    readonly property string websiteLink: "http://sites.google.com/view/flicsfacts/home"

    id: root
    visible: true
    width: windowWidth
    height: windowHeight
    color: Material.background
    title: MovieViewManager.appNameVersion

    MainDrawer {
        id: mainDrawerId
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: MainPage {
        }
    }
}
