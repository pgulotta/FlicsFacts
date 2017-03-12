import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

import "../fam"

Page {
    id: upcomingPageId
    header: UpcomingMoviesToolBar {
    }

    SwipeView {
        id: upcomingMoviesSwipeViewId
        anchors.fill: parent
        clip: true

        currentIndex: 0
        Repeater {
            id: upcoingMoviesModelId
            model: sortedUpcomingMoviesResponses
            delegate: movieResponseDelegateId
        }
    }
    PageIndicator {
        count: upcomingMoviesSwipeViewId.count
        currentIndex: upcomingMoviesSwipeViewId.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    MovieResponseDelegate {
        id: movieResponseDelegateId
    }

    Connections {
        target: MovieViewManager
        onResponseReceived: {
            upcomingMoviesSwipeViewId.currentIndex = responseId
        }
    }

    Component.onCompleted: MovieViewManager.queryUpcomongMovies()

    function onBackButtonSelected() {
        StackView.view.pop()
    }
}
