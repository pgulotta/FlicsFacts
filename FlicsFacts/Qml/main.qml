import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1
import "../fam"

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

    id: rootId
    visible: true
    width: windowWidth
    height: windowHeight
    color: Material.background
    title: MovieViewManager.appNameVersion

    header: ToolBar {
        id: topToolbarId
        Material.elevation: 4
        RowLayout {
            anchors.fill: parent
            ToolButton {
                id: menuToolbarId
                anchors.left: parent.left
                anchors.leftMargin: textBorderWidth
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/menu.png"
                }
                onClicked: drawerId.open()
            }
            Rectangle {
                id: searchTextRectId
                width: isPortraitMode ? .4 * rootId.width : .5 * rootId.width
                height: tabHeight
                radius: 4
                anchors.left: menuToolbarId.right
                anchors.leftMargin: textBorderWidth
                border.width: textBorderWidth
                border.color: Material.accent
                TextField {
                    id: titleRequestId
                    height: tabHeight + 12
                    focus: true
                    placeholderText: qsTr("Movie Title")
                    color: Material.primary
                    font.pointSize: fontSizeSmall
                    anchors.top: searchTextRectId.top
                    anchors.left: searchTextRectId.left
                    anchors.leftMargin: textMargin
                    anchors.right: searchTextRectId.right
                    anchors.rightMargin: textMargin
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
                visible: searchResponseModel.count !== 0
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/remove.png"
                }
                onClicked: {
                    MovieViewManager.removeSelectedMovie(movieSearchResultsId.movieIndex)
                    onFocusChanged: Qt.inputMethod.hide()
                }
            }
            ToolButton {
                id: shareButtonId
                anchors.left: removeButtonId.right
                visible: isAndroidPlatform && searchResponseModel.count !== 0
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/share.png"
                }
                onClicked: {
                    MovieViewManager.shareMovieResponses()
                    onFocusChanged: Qt.inputMethod.hide()
                }
            }
            ToolButton {
                id: moreButtonId
                anchors.right: parent.right
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/more.png"
                }
                onClicked: showAboutId.open()
            }
        }
    }

    Drawer {
        id: drawerId
        width: isPortraitMode ? rootId.width / 2 : rootId.width / 3
        height: rootId.height
    }

    MovieSearchResults {
        id: movieSearchResultsId
        visible: true
        FloatingActionMenu {
            famIconColor: Material.accent
        }

    }

    ShowAbout {
        id: showAboutId
    }

    function getCurrentSearchMovieTitle() {
        console.log("getCurrentSearchMovieTitle")
        return ( searchResponseModel.count === 0 || movieSearchResultsId === undefined || movieSearchResultsId.movieIndex === undefined)  ?
                    "" :  searchResponseModel.get(movieSearchResultsId.movieIndex)
    }

    function processSearchRequest() {
        MovieViewManager.findFlicSelected(titleRequestId.text)
        titleRequestId.text = ""
    }
}
