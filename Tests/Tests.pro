QT   += testlib \
    qml \
    quick \
    quickcontrols2 \
    widgets \
    network \

android {
    QT += androidextras
}

QT     -= gui

TARGET = testflicsfacts
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += qt warn_on  c++14

TEMPLATE = app

RESOURCES += \
    resources.qrc

HEADERS += \
    ../FlicsFacts/Model/movieresponse.hpp \
    ../FlicsFacts/Model/sortedmovieresponsesmodel.hpp \
    ../FlicsFacts/Controller/moviesearchparser.hpp \
    ../FlicsFacts/Controller/shareresponsesformatter.hpp \
    ../FlicsFacts/Controller/movieviewmanager.hpp \
    ../FlicsFacts/Controller/shareclient.hpp \
    ../FlicsFacts/fam/qqmlobjectlistmodel.hpp


SOURCES += \
    testflicsfacts.cpp \
    ../FlicsFacts/Controller/moviesearchparser.cpp \
    ../FlicsFacts/Controller/shareresponsesformatter.cpp \
    ../FlicsFacts/Controller/movieviewmanager.cpp \
    ../FlicsFacts/Controller/shareclient.cpp



DEFINES += SRCDIR=\\\"$$PWD/\\\"




