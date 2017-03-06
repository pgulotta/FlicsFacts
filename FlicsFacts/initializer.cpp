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
    mQmlApplicationEngine.rootContext()->setContextProperty("searchResponseModel", mMovieViewManager.searchResponseModel() );
    mQmlApplicationEngine.rootContext()->setContextProperty("MovieViewManager",&mMovieViewManager);
    mQmlApplicationEngine.load(QUrl(QStringLiteral("qrc:/Qml/main.qml")));

#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

}
