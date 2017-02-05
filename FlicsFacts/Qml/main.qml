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
    readonly property string omdbLink: "http://www.omdbapi.com/"
    readonly property string qtLink: "http://www.qt.io/"
    readonly property string websiteLink: "http://sites.google.com/view/flicsfacts-26apps/home"
    property alias tabViewCurrentIndex: movieResultsId.currentIndex
    property alias tabViewCurrentTitle: movieResultsId.currentTitle

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
            Rectangle {
                id: searchTextRectId
                width: isPortraitMode ? .4 * rootId.width : .5 * rootId.width
                height: tabHeight
                radius: 4
                anchors.left: parent.left
                anchors.leftMargin: spacingIndent
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
                    text: MovieViewManager.titleRequest

                    Keys.onReturnPressed: {
                        Qt.inputMethod.hide()
                        processSearchRequest()
                    }
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
                }
            }
            ToolButton {
                id: removeButtonId
                anchors.left: searchButtonId.right
                visible: tabViewCurrentTitle !== ""
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/remove.png"
                }
                onClicked: {
                    var movieCount = MovieViewManager.removeSelectedMovie(
                                tabViewCurrentIndex)
                    if (movieCount === 0)
                        tabViewCurrentTitle = ""
                    movieResultsId.deleteTab(tabViewCurrentIndex)
                    movieResultsId.refreshTab()
                }
            }
            ToolButton {
                id: shareButtonId
                anchors.left: removeButtonId.right
                visible: tabViewCurrentTitle !== "" && isAndroidPlatform
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/share.png"
                }
                onClicked: {
                    MovieViewManager.shareMovieResponses()
                }
            }
            ToolButton {
                id: menuButtonId
                anchors.right: parent.right
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "qrc:/Images/menu.png"
                }
                onClicked: showAboutId.open()
            }
        }
    }

    MovieResults {
        id: movieResultsId
    }

    ShowAbout {
        id: showAboutId
    }

    function processSearchRequest() {
        MovieViewManager.findFlicSelected(titleRequestId.text)
        titleRequestId.text = ""
    }
}
