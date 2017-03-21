#include <QApplication>
#include "initializer.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QApplication::quit);
    Initializer initializer;
    return app.exec();
}
