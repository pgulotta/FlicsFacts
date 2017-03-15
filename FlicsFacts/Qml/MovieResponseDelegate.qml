import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

Component {
    id: movieResponseDelegateId
    Item {
        id: movieResponseItemId

        readonly property string expandPosterImage: "expandImage"
        property int gridColumnCount: isPortraitMode ? 2 : 4
        property int flickableItemWidth: isPortraitMode ? windowWidth * .65 : windowWidth * .75
        property int plotItemHeight: isPortraitMode ? windowHeight * .35 : windowHeight * .2
        property int actorsItemHeight: isPortraitMode ? plotItemHeight * .3 : plotItemHeight * .5
        property int firstColumnWidth: isPortraitMode ? windowWidth
                                                        / (gridColumnCount + 2) : windowWidth
                                                        / (gridColumnCount + 2)

        property int imageDimension: isPortraitMode ? windowHeight : windowHeight

        Grid {
            id: gridTopId
            anchors.top: movieResponseItemId.top
            anchors.topMargin: textMargin
            anchors.left: movieResponseItemId.left
            anchors.leftMargin: textMargin
            columns: gridColumnCount
            rowSpacing: textMargin
            columnSpacing: textMargin
            verticalItemAlignment: Grid.AlignBottom
            visible: model.runtime !== ""
            opacity: 1
            state: ""

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
                text: qsTr("Released")
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
            anchors.left: movieResponseItemId.left
            anchors.leftMargin: textMargin
            columns: 2
            columnSpacing: textMargin
            rowSpacing: gridTopId.rowSpacing
            visible: gridTopId.visible

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
                width: windowWidth
                horizontalAlignment: "AlignHCenter"
                wrapMode: Label.Wrap
                color: Material.primary
                font.pointSize: fontSizeMedium
                text: model.status
            }
        }
        Image {
            id: posterImageId
            state: ""
            width: firstColumnWidth - 20
            height: firstColumnWidth - 20
            opacity: 1
            source: model.poster
            anchors.bottom: parent.bottom
            anchors.bottomMargin: textMargin
            anchors.left: parent.left
            anchors.leftMargin: textMargin
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    posterImageId.state = posterImageId.state
                            === expandPosterImage ? "" : expandPosterImage
                    console.log(" posterImageId.state=" + posterImageId.state)
                }
            }
            states: [
                State {
                    name: expandPosterImage
                    PropertyChanges {
                        target: posterImageId
                        width: imageDimension
                    }
                    PropertyChanges {
                        target: posterImageId
                        height: imageDimension
                    }
                    PropertyChanges {
                        target: gridTopId
                        opacity: 0
                    }
                },

                State {
                    name: ""
                    PropertyChanges {
                        target: posterImageId
                        width: firstColumnWidth - 20
                    }
                    PropertyChanges {
                        target: posterImageId
                        height: firstColumnWidth - 20
                    }
                    PropertyChanges {
                        target: gridTopId
                        opacity: 1
                    }
                }
            ]
            transitions: [
                Transition {
                    NumberAnimation {
                        target: posterImageId
                        properties: "width, height"
                        duration: 1000
                    }
                    NumberAnimation {
                        target: gridTopId
                        properties: "opacity"
                        duration: 1000
                    }
                }
            ]
        }
    }
}
