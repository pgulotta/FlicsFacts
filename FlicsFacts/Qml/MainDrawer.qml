import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1
import "../fam"

Drawer {
    id: drawerId
    width: isPortraitMode ? rootId.width / 2 : rootId.width / 3
    height: rootId.height

    Component {
        id: headerId
        Rectangle {
            width: parent.width
            height: width / 2
            Behavior on x {
                ColorAnimation {
                    duration: 1000
                }
            }
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "DarkSlateBlue"
                }
                GradientStop {
                    position: 0.25
                    color: "MediumSlateBlue"
                }
                GradientStop {
                    position: 0.50
                    color: "Lavender"
                }
                GradientStop {
                    position: 0.75
                    color: "MediumSlateBlue"
                }

                GradientStop {
                    position: 1.0
                    color: "DarkSlateBlue"
                }
            }
            TitleLabel {
                text: "FlicsFacts"
                anchors.centerIn: parent
            }
        }
    }
    ListView {
        id: listView
        currentIndex: -1
        anchors.fill: parent
        header: headerId

        focus: true

        delegate: ItemDelegate {
            width: parent.width
            text: model.title
            highlighted: ListView.isCurrentItem
            onClicked: {

                //                if (listView.currentIndex != index) {
                //                    listView.currentIndex = index
                //                    stackView.push(model.source)
                //                }
                showAboutId.open()
                drawerId.close()
            }
        }

        model: ListModel {
            ListElement {
                title: "Movie Search"
                source: "qrc:/Qml/MovieSearchResults.qml"
            }
            ListElement {
                title: "About"
                source: "qrc:/Qml/ShowAbout.qml"
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {
        }
    }
}
