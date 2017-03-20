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

        property int gridColumnCount: isPortraitMode ? 2 : 4
        property int flickableItemWidth: isPortraitMode ? windowWidth * .65 : windowWidth * .75
        property int plotItemHeight: isPortraitMode ? windowHeight * .3 : windowHeight * .2
        property int actorsItemHeight: isPortraitMode ? windowHeight * .15 : windowHeight * .10
        property int firstColumnWidth: isPortraitMode ? windowWidth
                                                        / (gridColumnCount + 2) : windowWidth
                                                        / (gridColumnCount + 2)
        property int imageDimension: firstColumnWidth - spacingIndent
        property int imageExpandedDimension: isPortraitMode ? windowWidth
                                                              - spacingIndent : windowHeight * .75
        property int imageTransitionInteralMS: 500
        property string expandPosterImage: "expandPosterImage"
        property string expandPosterImageState: ""

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
        Rectangle {
            id: posterImageId
            width: imageDimension
            height: imageDimension
            color: "indigo"
            radius: 4
            anchors.bottom: parent.bottom
            anchors.bottomMargin: textMargin * 2
            anchors.left: parent.left
            anchors.leftMargin: textMargin
            visible: model.poster !== ""
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    expandPosterImageState = (expandPosterImageState
                                              === expandPosterImage) ? "" : expandPosterImage
                }
            }
            Image {
                state: expandPosterImageState
                width: imageDimension
                height: imageDimension
                fillMode: Image.PreserveAspectCrop
                source: model.poster
                anchors.fill: parent
                anchors.margins: 4
                states: [
                    State {
                        name: expandPosterImage
                        PropertyChanges {
                            target: posterImageId
                            width: imageExpandedDimension
                        }
                        PropertyChanges {
                            target: posterImageId
                            height: imageExpandedDimension
                        }
                        PropertyChanges {
                            target: gridTopId
                            opacity: 0.25
                        }
                        PropertyChanges {
                            target: gridBottomId
                            opacity: 0.25
                        }
                    },

                    State {
                        name: ""
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
                            opacity: 1
                        }
                        PropertyChanges {
                            target: gridBottomId
                            opacity: 1
                        }
                    }
                ]
                transitions: [
                    Transition {
                        NumberAnimation {
                            target: posterImageId
                            properties: "width, height"
                            duration: imageTransitionInteralMS
                        }
                        NumberAnimation {
                            target: gridTopId
                            properties: "opacity"
                            duration: imageTransitionInteralMS
                        }
                        NumberAnimation {
                            target: gridBottomId
                            properties: "opacity"
                            duration: imageTransitionInteralMS
                        }
                    }
                ]
            }
        }
    }
}
