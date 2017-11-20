#include "initializer.hpp"
#include <QApplication>
#include <QQuickStyle>



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setApplicationName("FlicsFacts");
    QApplication::setOrganizationDomain("twentysixapps.com");
    QApplication::setOrganizationName("26Apps");
    QApplication::setApplicationVersion("1.18");
    QQuickStyle::setStyle("Material");
    QApplication app(argc, argv);
    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);
    Initializer initializer;
    return app.exec();
}
