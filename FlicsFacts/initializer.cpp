#include "initializer.hpp"

#include <QApplication>
#include <QQmlContext>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

#include "fam/floatingactions.hpp"

Initializer::Initializer(QObject *parent) :
    QObject{parent},
    mMovieViewManager{parent}
{
    QQmlApplicationEngine engine;

    FloatingActions::initialize(engine.rootContext(), &mQmlApplicationEngine);
    mQmlApplicationEngine.rootContext()->setContextProperty("movieSearchResponses", mMovieViewManager.movieSearchResponses() );
    mQmlApplicationEngine.rootContext()->setContextProperty("nowPlayingMoviesResponses", mMovieViewManager.nowPlayingMoviesResponses() );
    mQmlApplicationEngine.rootContext()->setContextProperty("sortedUpcomingMoviesResponses", mMovieViewManager.sortedUpcomingMoviesResponses() );
    mQmlApplicationEngine.rootContext()->setContextProperty("sortedNowPlayingMoviesResponses", mMovieViewManager.sortedNowPlayingMoviesResponses() );
    mQmlApplicationEngine.rootContext()->setContextProperty("MovieViewManager",&mMovieViewManager);
    mQmlApplicationEngine.load(QUrl(QStringLiteral("qrc:/Qml/main.qml")));

#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

}
