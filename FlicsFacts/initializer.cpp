#include "initializer.hpp"

#include <QApplication>
#include <QQmlContext>
#include <QQuickStyle>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif

#include "fam/floatingactions.hpp"

Initializer::Initializer(QObject *parent) :
    QObject{parent},
    mMovieViewManager{parent}
{

    QApplication::setApplicationName(tr("FlicsFacts"));
    QApplication::setOrganizationName("26Apps");
    QQuickStyle::setStyle("Material");
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
