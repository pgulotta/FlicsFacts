TEMPLATE = app

QT += qml \
    quick \
    quickcontrols2 \
    widgets \
    network \

CONFIG += c++14
#CONFIG += c++14 \
#	qtquickcompiler


include(./fam/fam.pri)

SOURCES += main.cpp \
    initializer.cpp \
    Controller/omdbresponseparser.cpp \
    Controller/movieviewmanager.cpp \
    Controller/shareclient.cpp \
    Controller/shareresponsesformatter.cpp

HEADERS += \
    initializer.hpp \
    Model/movieresponse.hpp \
    Controller/omdbresponseparser.hpp \
    Controller/movieviewmanager.hpp \
    Controller/shareclient.hpp \
    Controller/shareresponsesformatter.hpp


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
    Qml/main.qml \
    Qml/ShowAbout.qml \
    Qml/ShowMessage.qml \
    Qml/MovieResults.qml \
    Qml/TitleLabel.qml	\
    Qml/TextLabel.qml \	
    Qml/GridItemLabel.qml \
    Qml/GridTitleLabel.qml \
    Qml/HeadingLabel.qml \
	android/AndroidManifest.xml 
	
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




