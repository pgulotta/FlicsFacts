#include "shareresponsesformatter.hpp"
#include <QString>
#include <QDebug>


void formatField(const QString &key, const QString &value, QString &formattedText)
{
    formattedText += QString("%1=%2    ").arg(key).arg(value);
}

ShareResponsesFormatter::ShareResponsesFormatter(QObject *parent) :
    QObject{parent},
    mFormattedResponses{1024}
{
}

QString ShareResponsesFormatter::formatAsText(QQmlObjectListModel<MovieResponse>::const_iterator cbegin,
                                              QQmlObjectListModel<MovieResponse>::const_iterator cend)
{
    try {
        mFormattedResponses.clear();
        std::for_each(cbegin, cend, [&]  ( auto&& movieResponse)
        {
            if ( movieResponse->movieId() != MovieResponse::invalidMovieId() )
            {
                mFormattedResponses += QString("\r\n*****  %1    *****\r\n").arg(movieResponse->title());
                formatField(tr("Genre"), movieResponse->genre(), mFormattedResponses);
                formatField(tr("Year"), movieResponse->year(), mFormattedResponses);
                formatField(tr("Released"), movieResponse->released(), mFormattedResponses);
                formatField(tr("Runtime"), movieResponse->runtime(), mFormattedResponses);
                formatField(tr("Rated"), movieResponse->rating(), mFormattedResponses);
                formatField(tr("Popularity"), movieResponse->popularity(), mFormattedResponses);
                formatField(tr("Languages"), movieResponse->languages(), mFormattedResponses);
                mFormattedResponses += QString("\r\n");
                formatField(tr("Actors"), movieResponse->actors(), mFormattedResponses);
                if ( !movieResponse->websiteUrl().trimmed().isEmpty())
                    formatField(tr("Website"), movieResponse->websiteUrl(), mFormattedResponses);
                mFormattedResponses += QString("\r\n");
                formatField(tr("Plot"), movieResponse->plot(), mFormattedResponses);
                mFormattedResponses += QString("\r\n");
            }
        });
    }
    catch(std::exception const & e)
    {
        qDebug() << "ShareResponsesFormatter::queryMovieCredits formatAsText: " << e.what();
    }
    return mFormattedResponses;
}
