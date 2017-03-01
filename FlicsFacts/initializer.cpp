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
    // qmlRegisterType<MovieSearchResponse>("MovieSearchResponse",1,0,"MovieSearchResponse");
    mQmlApplicationEngine.rootContext()->setContextProperty("searchResponseModel", mMovieViewManager.searchResponseModel() );
    mQmlApplicationEngine.rootContext()->setContextProperty("MovieViewManager",&mMovieViewManager);
    mQmlApplicationEngine.load(QUrl(QStringLiteral("qrc:/Qml/main.qml")));
//    auto rootObjects = mQmlApplicationEngine.rootObjects();
//    if (rootObjects.count() < 1)
//    {
//        qDebug() << "Failed to load Qml.  Application is exiting";
//        app.exit();
//    }
#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

}
