import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.1
import "../fam"

Drawer {
    id: drawerId
    width: isPortraitMode ? root.width / 2 : root.width / 3
    height: root.height

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
                    position: 0.00;
                    color: "#483d8b";
                }
                GradientStop {
                    position: 0.50;
                    color: "#7b68ee";
                }
                GradientStop {
                    position: 1.00;
                    color: "#483d8b";
                }
            }
            TitleLabel {
                text: MovieViewManager.appName
                anchors.centerIn: parent
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
            highlighted: ListView.isCurrentItem
            onClicked: {
                if (drawerListViewId.currentIndex != index) {
                    drawerListViewId.currentIndex = index
                    stackView.push(model.source)
                    drawerListViewId.currentIndex =-1
                }
                mainDrawerId.close()
            }
        }
        model: ListModel {
            ListElement {
                title: "Movie Search"
                source: "qrc:/Qml/MovieSearchPage.qml"
            }
            ListElement {
                title: "About"
                source: "qrc:/Qml/AboutPage.qml"
            }
        }

        ScrollIndicator.vertical: ScrollIndicator {
        }


    }
}
