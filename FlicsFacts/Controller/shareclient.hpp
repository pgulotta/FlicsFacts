#pragma once
#include <QObject>



class ShareClient final : public QObject
{
Q_OBJECT

public slots:
    void setShare(const QString &shareContent) const;

signals:

public:
    explicit ShareClient(QObject *parent = 0) : QObject(parent){
    }
    explicit ShareClient(const ShareClient& rhs) = delete;
    ShareClient& operator= (const ShareClient& rhs) = delete;

private:

};
