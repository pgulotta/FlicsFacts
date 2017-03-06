import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

Page {
    id: movieSearchPageId

    signal backButtonSelected

    readonly property int tabAnimationDuration: 400
    readonly property int animationFromX: 0
    readonly property int flickableItemWidth: isPortraitMode ? windowWidth * .65 : windowWidth * .75
    readonly property int plotItemHeight: isPortraitMode ? windowHeight * .35 : windowHeight * .2
    readonly property int actorsItemHeight: isPortraitMode ? plotItemHeight
                                                             * .4 : plotItemHeight * .5
    readonly property int firstColumnWidth: isPortraitMode ? windowWidth / (gridColumnCount + 2) : windowWidth / (gridColumnCount + 2)
    property string currentTitle: ""
    property alias movieIndex: swipeViewId.currentIndex


    //  anchors.fill: parent
    header: ToolBar {
        id: movieSearchToolBarId
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
        Rectangle {
            id: searchTextRectId
            width: .5 * windowWidth
            height: parent.height * .8
            radius: 4
            anchors.left: backToolButtonId.right
            anchors.leftMargin: textBorderWidth
            anchors.top: parent.top
            border.width: textBorderWidth
            border.color: Material.accent
            TextField {
                id: titleRequestId
                height: tabHeight + 12
                focus: true
                placeholderText: qsTr("Movie Title")
                color: Material.primary
                font.pointSize: fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                anchors.left: searchTextRectId.left
                anchors.leftMargin: textMargin
                anchors.right: searchTextRectId.right
                anchors.rightMargin: textMargin
                anchors.top: searchTextRectId.top
                anchors.topMargin: 8
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
                            movieSearchPageId.movieIndex)
                onFocusChanged: Qt.inputMethod.hide()
            }
        }
    }

    SwipeView {
        id: swipeViewId
        anchors.fill: parent
        clip: true
        currentIndex: 0
        Repeater {
            id: searchResponseModelId
            model: searchResponseModel
            delegate: searchResponseDelegateId
        }
        onCurrentIndexChanged: MovieViewManager.tryQueryMovieSearch(
                                   currentIndex)
    }
    PageIndicator {
        count: swipeViewId.count
        currentIndex: swipeViewId.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Component {
        id: searchResponseDelegateId
        Item {
            Grid {
                id: gridTopId
                columns: gridColumnCount
                rowSpacing: textMargin
                columnSpacing: textMargin
                verticalItemAlignment: Grid.AlignBottom
                visible: model.runtime !== ""
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
                    text: model.title
                    font.bold: true
                }
                GridTitleLabel {
                    text: qsTr("Website")
                }
                Text {
                    text: model.website
                    font.pointSize: fontSizeMedium
                    onLinkActivated: Qt.openUrlExternally(model.websiteUrl)
                    color: Material.primary
                }
                GridTitleLabel {
                    text: qsTr("Year")
                }
                GridItemLabel {
                    text: model.year
                }
                GridTitleLabel {
                    text: "Release"
                }
                GridItemLabel {
                    text: model.released
                }
                GridTitleLabel {
                    text: qsTr("Popularity")
                }
                GridItemLabel {
                    text: model.popularity
                }
                GridTitleLabel {
                    text: qsTr("Rated")
                }
                GridItemLabel {
                    text: model.rating
                }
                GridTitleLabel {
                    text: qsTr("Languages")
                }
                GridItemLabel {
                    text: model.languages
                }
                GridTitleLabel {
                    text: qsTr("Runtime")
                }
                Text {
                    id: runtimeId
                    rightPadding: columnItemIndent
                    text: model.runtime
                    color: Material.primary
                    font.pointSize: fontSizeMedium
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
                    text: model.genre
                }
                GridTitleLabel {
                    text: qsTr("Actors")
                }
                Flickable {
                    contentHeight: actorsId.height
                    width: actorsId.width
                    height: actorsItemHeight
                    clip: true
                    GridItemLabel {
                        id: actorsId
                        text: model.actors
                        width: flickableItemWidth
                        wrapMode: Label.Wrap
                    }
                }

                GridTitleLabel {
                    text: qsTr("Plot")
                }
                Flickable {
                    contentHeight: plotId.height
                    width: plotId.width
                    height: plotItemHeight
                    clip: true
                    GridItemLabel {
                        id: plotId
                        text: model.plot
                        width: flickableItemWidth
                        wrapMode: Label.Wrap
                    }
                }
            }
            Row {
                anchors.top: gridBottomId.top
                Label {
                    width: movieSearchPageId.width
                    horizontalAlignment: "AlignHCenter"
                    wrapMode: Label.Wrap
                    color: Material.primary
                    font.pointSize: fontSizeMedium
                    text: model.status
                }
            }
        }
    }

    FloatingActionMenu {
        id: famShareId
        visible: searchResponseModel.count !== 0
        famIconColor: "DarkSlateBlue"
        famImage: "qrc:/Images/more.png"
        labelWidth: 180
        famLabelBackColor: "white"
        onButtonItemSelected: {
            switch (modelIndex) {
            case 0:
                MovieViewManager.shareMovieResponses()
                break
            case 1:
                MovieViewManager.removeAllMovieSearchResponses()
                break
            default:
                console.log("onButtonItemSelected error")
                break
            }
        }

        floatingActionButtons: ListModel {
            ListElement {
                description: qsTr("Share All Movies")
                iconUrl: "qrc:/Images/share.png"
                iconColor: "DarkSlateBlue"
            }
            ListElement {
                description: "Remove Movie Searches"
                iconUrl: "qrc:/Images/deleteall.png"
                iconColor: "DarkSlateBlue"
            }
        }
    }

    Connections {
        target: MovieViewManager
        onResponseReceived: {
            swipeViewId.currentIndex = responseId
        }
    }
    Connections {
        target: MovieViewManager
        onDisplayTextMessage: showMessageId.show(title, message)
    }
    Component.onCompleted: {
        titleRequestId.forceActiveFocus()
        Qt.inputMethod.hide()
    }

    ShowMessage {
        id: showMessageId
    }

    function processSearchRequest() {
        MovieViewManager.findFlicSelected(titleRequestId.text)
        titleRequestId.text = ""
    }

    onBackButtonSelected: StackView.view.pop()
}
