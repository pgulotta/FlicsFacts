TEMPLATE = app

QT += qml \
    quick \
    quickcontrols2 \
    widgets \
    network \


CONFIG += c++14 \
    qtquickcompiler


include(./fam/fam.pri)

SOURCES += main.cpp \
    initializer.cpp \
    Controller/movieviewmanager.cpp \
    Controller/shareclient.cpp \
    Controller/shareresponsesformatter.cpp \
    Controller/tmdbresponseparser.cpp

HEADERS += \
    initializer.hpp \
    Controller/movieviewmanager.hpp \
    Controller/shareclient.hpp \
    Controller/shareresponsesformatter.hpp \
    Model/moviesearchresponse.hpp \
    Controller/tmdbresponseparser.hpp


DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    android/res/drawable/splash.png \
    android/res/drawable/splashscreen.xml \
    android/src/com/twentysixapps/flicsfacts/MovieShareIntent.java \
	android/AndroidManifest.xml \ 

	
RESOURCES += qml.qrc

ICON = Resources/icon.png

QML_IMPORT_PATH += $$PWD

DEFINES += QT_USE_QSTRINGBUILDER \
    QT_DEPRECATED_WARNINGS


win32: RC_FILE = Resources/FlicsFacts.rc
gcc|clang {
    QMAKE_CXXFLAGS += -std=c++1y
}

android {
    QT += androidextras
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

message(Qt version: $$[QT_VERSION])




