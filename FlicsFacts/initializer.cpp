#include "initializer.hpp"

#include <QApplication>
#include <QQmlContext>
#include <QQuickStyle>

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#endif


Initializer::Initializer(QObject *parent) :
    QObject{parent},
    mMovieViewManager{parent}
{

    QApplication::setApplicationName(tr("FlicsFacts"));
    QApplication::setOrganizationName("26Apps");
    QQuickStyle::setStyle("Material");

    mQmlApplicationEngine.rootContext()->setContextProperty("MovieViewManager",&mMovieViewManager);
    mQmlApplicationEngine.load(QUrl(QStringLiteral("qrc:/Qml/main.qml")));

#ifdef Q_OS_ANDROID
    QtAndroid::hideSplashScreen();
#endif

}
