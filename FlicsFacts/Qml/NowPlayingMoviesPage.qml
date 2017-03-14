import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

Page {
    id: movieResponsePageId

    header: TitledToolBar {
        pageTitle: qsTr("Movies Now Playing")
    }

    SwipeView {
        id: nowPlayingMoviesSwipeViewId
        anchors.fill: parent
        clip: true
        currentIndex: 0
        Repeater {
            id: movieResponseModelId
            model: sortedNowPlayingMoviesResponses
            delegate: movieResponseDelegateId
        }
    }
    PageIndicator {
        count: nowPlayingMoviesSwipeViewId.count
        currentIndex: nowPlayingMoviesSwipeViewId.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MovieResponseDelegate {
        id: movieResponseDelegateId
    }

    Connections {
        target: MovieViewManager
        onResponseReceived: nowPlayingMoviesSwipeViewId.currentIndex = responseId
    }

    Component.onCompleted: MovieViewManager.queryNowPlayingMovies()
}
