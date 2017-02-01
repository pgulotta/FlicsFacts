#pragma once

#include <../FlicsFacts/Model/movieresponse.hpp>
#include <QObject>
#include <vector>
#include <iterator>
#include <memory>

class QString;

class ShareResponsesFormatter : public QObject
{
Q_OBJECT

public slots:

signals:

public:
    explicit ShareResponsesFormatter(QObject *parent = 0);
    explicit ShareResponsesFormatter(const ShareResponsesFormatter& rhs) = delete;
    ShareResponsesFormatter& operator= (const ShareResponsesFormatter& rhs) = delete;
    QString formatAsText(std::vector<std::unique_ptr<MovieResponse> >::const_iterator begin, std::vector<std::unique_ptr<MovieResponse> >::const_iterator end);

private:
    static inline void formatField(const QString &key, const QString &value, QString &formattedText);

private:
    QString mFormattedResponses;

};

