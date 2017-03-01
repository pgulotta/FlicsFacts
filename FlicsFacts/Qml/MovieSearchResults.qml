import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

Pane {
    id: paneId
    anchors.fill: parent
    readonly property int tabAnimationDuration: 400
    readonly property int animationFromX: 0
    readonly property int flickableItemWidth: isPortraitMode ? windowWidth * .65 : windowWidth * .75
    readonly property int plotItemHeight: isPortraitMode ? windowHeight * .35 : windowHeight * .2
    readonly property int actorsItemHeight: isPortraitMode ? plotItemHeight
                                                             * .4 : plotItemHeight * .5
    readonly property int firstColumnWidth: isPortraitMode ? windowWidth / (gridColumnCount + 2) : windowWidth / (gridColumnCount + 2)
    property string currentTitle: ""

    property alias movieIndex: swipeViewId.currentIndex

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
                visible: model.title !== ""
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
                    width: paneId.width
                    horizontalAlignment: "AlignHCenter"
                    wrapMode: Label.Wrap
                    color: Material.primary
                    font.pointSize: fontSizeMedium
                    text: model.status
                }
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


}
