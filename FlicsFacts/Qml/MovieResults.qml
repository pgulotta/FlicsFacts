import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.1

TabView {
    id: tabViewId
    anchors.fill: parent
    tabPosition: Qt.TopLeftCorner
    anchors.top: parent.top
    anchors.left: parent.left

    property int tabAnimationDuration: 400
    property int animationFromX: 0
    property string currentTitle: ""
    property int tabItemWidth: isPortraitMode ? windowWidth * .7 : windowWidth * .8
    property int tabItemHeight: isPortraitMode ? windowHeight * .35 : windowHeight * .20
    readonly property int firstColumnWidth: isPortraitMode ? windowWidth / (gridColumnCount + 3) : windowWidth / (gridColumnCount + 2)

    style: TabViewStyle {
        tabOverlap: 0
        frameOverlap: 0
        tabsMovable: false
        tab: Rectangle {
            color: styleData.selected ? Material.primary : Material.accent
            border.color: Material.foreground
            implicitWidth: Math.max(text.width + 4, 80)
            implicitHeight: tabHeight
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                verticalAlignment: Text.AlignVCenter
                font.pointSize: fontSizeMedium
                padding: textMargin
                color: styleData.selected ? Material.background : Material.primary
            }
        }
        frame: Rectangle {
            id: backColorRectId
            color: Material.background
        }
    }

    Component {
        id: tabComponentId

        Item {
            anchors.top: parent === null ? tabComponentId.top : parent.top
            anchors.topMargin: tabHeight + textMargin
            anchors.left: parent === null ? tabComponentId.left : parent.left
            anchors.leftMargin: textMargin
            Grid {
                id: gridTopId
                columns: gridColumnCount
                rowSpacing: textMargin
                columnSpacing: textMargin
                verticalItemAlignment: Grid.AlignBottom
                visible: MovieViewManager.title(currentIndex) !== ""
                NumberAnimation on y {
                    easing.type: Easing.Linear
                    from: -windowHeight
                    to: 0
                    duration: tabAnimationDuration
                }
                GridTitleLabel {
                    text: qsTr("Title")
                }
                GridItemLabel {
                    text: MovieViewManager.title(currentIndex)
                }
                GridTitleLabel {
                    text: qsTr("Website")
                }
                Text {
                    text: MovieViewManager.website(currentIndex)
                          === "N/A" ? " " : "<a href=\"" + MovieViewManager.website(
                                          currentIndex) + "\">Go to ...</a>"
                    font.pointSize: fontSizeMedium
                    onLinkActivated: Qt.openUrlExternally(
                                         MovieViewManager.website(currentIndex))
                    color: Material.primary
                }
                GridTitleLabel {
                    text: qsTr("Year")
                }
                GridItemLabel {
                    text: MovieViewManager.year(currentIndex)
                }
                GridTitleLabel {
                    text: "Release"
                }
                GridItemLabel {
                    text: MovieViewManager.released(currentIndex)
                }
                GridTitleLabel {
                    text: qsTr("Runtime")
                }
                Text {
                    id: runtimeId
                    rightPadding: columnItemIndent
                    text: MovieViewManager.runtime(currentIndex)
                    color: Material.primary
                    font.pointSize: fontSizeMedium
                }
                GridTitleLabel {
                    text: qsTr("Rated")
                }
                GridItemLabel {
                    text: MovieViewManager.rated(currentIndex)
                }
                GridTitleLabel {
                    text: qsTr("Tomatoes")
                }
                GridItemLabel {
                    text: MovieViewManager.tomatoRating(currentIndex)
                }
                GridTitleLabel {
                    text: qsTr("IMDB")
                }
                GridItemLabel {
                    text: MovieViewManager.imdbRating(currentIndex)
                }
            }
            Grid {
                id: gridBottomId
                anchors.top: gridTopId.bottom
                anchors.topMargin: textMargin
                columns: 2
                columnSpacing: textMargin
                rowSpacing: gridTopId.rowSpacing
                visible: gridTopId.visible
                NumberAnimation on y {
                    easing.type: Easing.Linear
                    from: -windowHeight
                    to: 0
                    duration: tabAnimationDuration
                }
                GridTitleLabel {
                    text: qsTr("Genre")

                }
                GridItemLabel {
                    text: MovieViewManager.genre(currentIndex)
                }
                GridTitleLabel {
                    text: qsTr("Actors")
                }
                GridItemLabel {
                    wrapMode: Label.Wrap
                    width: tabItemWidth
                    text: MovieViewManager.actors(currentIndex)
                }

                GridTitleLabel {
                    text: qsTr("Plot")
                }

                Flickable {
                    contentHeight: plotId.height
                    width: plotId.width
                    height: tabItemHeight
                    clip: true
                    GridItemLabel {
                        id: plotId
                        text: MovieViewManager.plot(currentIndex)
                        width: tabItemWidth
                        wrapMode: Label.Wrap
                    }
                }
            }
            Row {
                anchors.top: gridBottomId.bottom
                anchors.topMargin: textMargin
                Label {
                    width: tabViewId.width
                    horizontalAlignment: "AlignHCenter"
                    wrapMode: Label.Wrap
                    color: Material.primary
                    font.pointSize: fontSizeMedium
                    text: MovieViewManager.status(currentIndex)
                }
            }
        }
    }

    ShowMessage {
        id: showMessageId
    }

    Component.onCompleted: {
        Qt.inputMethod.hide()
    }

    onCurrentIndexChanged: {
        var status = MovieViewManager.status(currentIndex)
        if (status.startsWith("This request was unsuccessful.")) {

            var currentTab = getTab(currentIndex)
            if (currentTab !== undefined) {
                MovieViewManager.refreshSelectedMovie(currentIndex,
                                                      currentTab.title)
            }
        } else {
            refreshTab()
        }
    }
    Connections {
        target: MovieViewManager
        onResponseReceived: {
            currentIndex = responseId
            var currentTab = getTab(currentIndex)
            selectTab(currentTab)
        }
    }
    Connections {
        target: MovieViewManager
        onDisplayTextMessage: showMessageId.show(title, message)
    }

    Connections {
        target: MovieViewManager
        onRequestCreated: {
            var tab = tabViewId.insertTab(responseId, tabTitle, tabComponentId)
            currentIndex = responseId
            selectTab(tab)
        }
    }

    function deleteTab(responseId) {
        if (responseId >= 0 && responseId < tabViewId.count) {
            tabViewId.removeTab(responseId)
        }
    }

    function selectTab(currentTab) {
        if (currentTab === undefined) {
            currentTitle = ""
        } else {
            currentTitle = currentTab.title
            currentTab.active = false
            currentTab.active = true
        }
    }

    function refreshTab() {
        var currentTab = getTab(currentIndex)
        selectTab(currentTab)
    }

    function getMovieTitle(responseId) {
        var currentTab = getTab(responseId)
        movieTitle = (currentTab === undefined) ? "" : currentTab.title
        return movieTitle
    }
}
