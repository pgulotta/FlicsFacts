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
    ../FlicsFacts/Model/moviesearchresponse.hpp \
    ../FlicsFacts/Controller/omdbresponseparser.hpp \
    ../FlicsFacts/Controller/shareresponsesformatter.hpp \
    ../FlicsFacts/Controller/movieviewmanager.hpp \
    ../FlicsFacts/Controller/shareclient.hpp \
    ../FlicsFacts/fam/qqmlobjectlistmodel.hpp


SOURCES += \
    testflicsfacts.cpp \
    ../FlicsFacts/Controller/omdbresponseparser.cpp \
    ../FlicsFacts/Controller/shareresponsesformatter.cpp \
    ../FlicsFacts/Controller/movieviewmanager.cpp \
    ../FlicsFacts/Controller/shareclient.cpp



DEFINES += SRCDIR=\\\"$$PWD/\\\"




