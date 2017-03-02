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

    signal menuSelected(string contextId, bool isClosed)
    signal buttonSelected(string contextId, int modelIndex)

    header: ToolBar {
        id: topToolbarId
        Material.elevation: 4
        RowLayout {
            anchors.fill: parent
            //            ToolButton {
            //                id: menuToolbarId
            //                visible: true
            //                anchors.left: parent.left
            //                anchors.leftMargin: textBorderWidth
            //                contentItem: Image {
            //                    fillMode: Image.Pad
            //                    horizontalAlignment: Image.AlignHCenter
            //                    verticalAlignment: Image.AlignVCenter
            //                    source: "qrc:/Images/menu.png"
            //                }
            //                onClicked: drawerId.open()
            //            }
            Rectangle {
                id: searchTextRectId
                width: .5 * rootId.width
                height: tabHeight
                radius: 4
                //anchors.left: menuToolbarId.right
                //anchors.leftMargin: textBorderWidth
                anchors.left: parent.left
                anchors.leftMargin: textMargin
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
                    MovieViewManager.removeSelectedMovie(
                                movieSearchResultsId.movieIndex)
                    onFocusChanged: Qt.inputMethod.hide()
                }
            }
        }
    }

    //    Drawer {
    //        id: drawerId
    //        width: isPortraitMode ? rootId.width / 2 : rootId.width / 3
    //        height: rootId.height
    //    }
    MovieSearchResults {
        id: movieSearchResultsId
        visible: true
        FloatingActionMenu {
            id: famShareId
            visible: searchResponseModel.count !== 0
            famIconColor: "MediumPurple"
            famImage: "qrc:/Images/more.png"
            labelWidth: 160
            famLabelBackColor: Material.background
            onButtonItemSelected: {
                switch (modelIndex) {
                case 0:
                    MovieViewManager.shareMovieResponses()
                    break
                case 1:
                    MovieViewManager.removeAllMovieSearchResponses()
                    break
                case 2:
                    showAboutId.open()
                    break
                default:
                    console.log("onButtonItemSelected error" )
                    break
                }
            }

            floatingActionButtons: ListModel {
                ListElement {
                    description: "Share All Movies"
                    iconUrl: "qrc:/Images/share.png"
                    iconColor: "MediumPurple"
                }
                ListElement {
                    description: "Remove Movie Searches"
                    iconUrl: "qrc:/Images/deleteall.png"
                    iconColor: "MediumPurple"
                }
                ListElement {
                    description: "About"
                    iconUrl: "qrc:/Images/credits.png"
                    iconColor: "MediumPurple"
                }
            }
        }
    }
    ShowAbout {
        id: showAboutId
    }

    function processSearchRequest() {
        MovieViewManager.findFlicSelected(titleRequestId.text)
        titleRequestId.text = ""
    }
}
