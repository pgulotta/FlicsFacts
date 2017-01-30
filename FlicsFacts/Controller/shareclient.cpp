#include "shareclient.hpp"

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>

void ShareClient::setShare(const QString &shareContent) const
{
    QAndroidJniObject content = QAndroidJniObject::fromString(shareContent);
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniObject::callStaticMethod<void>
        ("com/twentysixapps/flicsfacts/MovieShareIntent",
        "startShareActivity",
        "(Ljava/lang/String;"
        "Lorg/qtproject/qt5/android/bindings/QtActivity;)V",
        content.object<jstring>(),
        activity.object<jobject>());
}
#else

void ShareClient::setShare(const QString &) const
{
}


#endif

