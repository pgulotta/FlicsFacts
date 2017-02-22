#include <QApplication>
#include "initializer.hpp"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    Initializer initializer;
    return app.exec();
}
