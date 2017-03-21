#pragma once

#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include <../FlicsFacts/Model/movieresponse.hpp>
#include <QObject>
#include <vector>
#include <iterator>
#include <memory>

class QString;

class ShareResponsesFormatter final : public QObject
{
Q_OBJECT

public slots:

signals:

public:
    explicit ShareResponsesFormatter(QObject *parent = 0);
    explicit ShareResponsesFormatter(const ShareResponsesFormatter& rhs) = delete;
    ShareResponsesFormatter& operator= (const ShareResponsesFormatter& rhs) = delete;
    QString formatAsText(QQmlObjectListModel<MovieResponse>::const_iterator begin, QQmlObjectListModel<MovieResponse>::const_iterator end);

private:
    QString mFormattedResponses;

};

