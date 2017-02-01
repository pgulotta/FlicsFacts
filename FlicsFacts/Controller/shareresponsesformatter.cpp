#include "shareresponsesformatter.hpp"
#include <QStringBuilder>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextDocumentWriter>
#include <QDebug>


ShareResponsesFormatter::ShareResponsesFormatter(QObject *parent) :
    QObject(parent)
{
}

void ShareResponsesFormatter::formatField(const QString &key, const QString &value, QString &formattedText)
{
    formattedText += QString("%1=%2    ").arg(key).arg(value);
}

QString ShareResponsesFormatter::formatAsText(std::vector<std::unique_ptr<MovieResponse> >::const_iterator cbegin,
                                              std::vector<std::unique_ptr<MovieResponse> >::const_iterator cend)
{
    QString text(1024);
    std::for_each(cbegin, cend, [&]  (auto&& movieResponse)
    {
        if ( movieResponse->Title.size() >0)
        {
            text += QString("\r\n*****  %1    *****\r\n").arg(movieResponse->Title);
            text += QString("\r\n*****  %1    *****\r\n").arg(movieResponse->Title);
            formatField(tr("Genre"), movieResponse->Genre, text);
            formatField(tr("Year"), movieResponse->Year, text);
            formatField(tr("Released"), movieResponse->Released, text);
            formatField(tr("Runtime"), movieResponse->Runtime, text);
            formatField(tr("ImdbRating"), movieResponse->ImdbRating, text);
            formatField(tr("TomatoRating"), movieResponse->TomatoRating, text);
            text += QString("\r\n");
            formatField(tr("Actors"), movieResponse->Actors, text);
            formatField(tr("Website"), movieResponse->Website, text);
            text += QString("\r\n");
            formatField(tr("Plot"), movieResponse->Plot, text);
            text += QString("\r\n");
        }
    });
    mFormattedResponses = text;
    return mFormattedResponses;
}
