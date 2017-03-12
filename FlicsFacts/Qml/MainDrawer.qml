import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1

Drawer {
    id: mainDrawerId
    width: isPortraitMode ? rootId.width * .7 : rootId.width * .4
    height: rootId.height

    Component {
        id: headerId
        Rectangle {
            width: parent.width
            height: width / 2

            gradient: Gradient {
                GradientStop {
                    position: 0.00
                    color: "DarkSlateBlue"
                }
                GradientStop {
                    position: 0.50
                    color: "#bfbff2"
                }
                GradientStop {
                    position: 1.00
                    color: "DarkSlateBlue"
                }
            }
            TitleLabel {
                text: MovieViewManager.appName
                anchors.centerIn: parent
                color: "DarkSlateBlue"
            }
        }
    }

    ListView {
        id: drawerListViewId
        currentIndex: -1
        anchors.fill: parent
        header: headerId
        focus: true

        delegate: ItemDelegate {
            width: parent.width
            text: model.title
            highlighted: false
            onClicked: {
                if (drawerListViewId.currentIndex != index) {
                    drawerListViewId.currentIndex = index
                    stackViewId.push(model.source)
                }
                drawerListViewId.currentIndex = -1
                mainDrawerId.close()
            }
            leftPadding: avatar.implicitWidth + spacingIndent
            Image {
                id: avatar
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: textMargin
                source: model.image
            }
        }
        model: ListModel {
            ListElement {
                title: qsTr("Movie Search")
                source: "qrc:/Qml/MovieResponsePage.qml"
                image: "qrc:/Images/movie_search.png"
            }
            ListElement {
                title: qsTr("Movies Now Playing")
                source: "qrc:/Qml/NowPlayingMoviesPage.qml"
                image: "qrc:/Images/play_movies.png"
            }
            ListElement {
                title: qsTr("Upcoming Movies")
                source: "qrc:/Qml/UpcomingMoviesPage.qml"
                image: "qrc:/Images/add_movies.png"
            }
            ListElement {
                title: qsTr("About")
                source: "qrc:/Qml/AboutPage.qml"
                image: "qrc:/Images/credits.png"
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {
        }
    }
}
