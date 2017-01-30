#include "shareresponsesformatter.hpp"
#include <QStringBuilder>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextDocumentWriter>
#include <QDebug>


ShareResponsesFormatter::ShareResponsesFormatter(QObject *parent) :
    QObject(parent),
    mFormattedResponses(1024)
{
}

void ShareResponsesFormatter::formatField(const QString &key, const QString &value, QString &formattedText)
{
    formattedText += QString("%1=%2    ").arg(key).arg(value);
}

QString ShareResponsesFormatter::formatAsText(std::vector<std::unique_ptr<MovieResponse> >::const_iterator begin, std::vector<std::unique_ptr<MovieResponse> >::const_iterator end)
{
    mFormattedResponses.clear();

    for (auto cit = begin; cit != end; ++cit) {
        auto movieResponse = (*cit).get();
        if ( movieResponse->Title.size() >0)
        {
            mFormattedResponses += QString("\r\n*****  %1    *****\r\n").arg(movieResponse->Title);
            formatField(tr("Genre"), movieResponse->Genre, mFormattedResponses);
            formatField(tr("Year"), movieResponse->Year, mFormattedResponses);
            formatField(tr("Released"), movieResponse->Released, mFormattedResponses);
            formatField(tr("Runtime"), movieResponse->Runtime, mFormattedResponses);
            formatField(tr("ImdbRating"), movieResponse->ImdbRating, mFormattedResponses);
            formatField(tr("TomatoRating"), movieResponse->TomatoRating, mFormattedResponses);
            mFormattedResponses += QString("\r\n");
            formatField(tr("Actors"), movieResponse->Actors, mFormattedResponses);
            formatField(tr("Website"), movieResponse->Website, mFormattedResponses);
            mFormattedResponses += QString("\r\n");
            formatField(tr("Plot"), movieResponse->Plot, mFormattedResponses);
            mFormattedResponses += QString("\r\n");
        }
    }
    return mFormattedResponses;
}
