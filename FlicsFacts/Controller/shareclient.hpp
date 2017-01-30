#pragma once
#include <QObject>



class ShareClient : public QObject
{
Q_OBJECT

public:
    Q_INVOKABLE void setShare(const QString &shareContent) const;

public slots:

signals:

public:
    explicit ShareClient(QObject *parent = 0) : QObject(parent){
    }
    explicit ShareClient(const ShareClient& rhs) = delete;
    ShareClient& operator= (const ShareClient& rhs) = delete;

private:

};
