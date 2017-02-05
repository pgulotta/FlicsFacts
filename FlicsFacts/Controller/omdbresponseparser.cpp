#include "omdbresponseparser.hpp"
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <exception>
#include <cassert>


OmdbResponseParser::OmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager) :
    QObject{parent},
    mMovieViewManager{movieViewManager}
{
}

void OmdbResponseParser::parse( const QByteArray& responseSource,  int responseId)
{
    bool success = false;
    try {
        QJsonDocument document = QJsonDocument::fromJson(responseSource);
        if ( !document.isNull() && !document.isEmpty())
        {
            QJsonObject jsonObject = document.object();
            if (jsonObject.contains(QStringLiteral("Title")))
            {
                if (jsonObject.contains(QStringLiteral("Title")))
                    mMovieViewManager.setTitle (responseId, jsonObject.value(QStringLiteral("Title")).toString());
                if (jsonObject.contains(QStringLiteral("Year")))
                    mMovieViewManager.setYear (responseId,  jsonObject.value(QStringLiteral("Year")).toString());
                if (jsonObject.contains(QStringLiteral("Rated")))
                    mMovieViewManager.setRated (responseId,  jsonObject.value(QStringLiteral("Rated")).toString());
                if (jsonObject.contains(QStringLiteral("Released")))
                    mMovieViewManager.setReleased ( responseId,  jsonObject.value(QStringLiteral("Released")).toString());
                if (jsonObject.contains(QStringLiteral("Runtime")))
                    mMovieViewManager.setRuntime ( responseId,  jsonObject.value(QStringLiteral("Runtime")).toString());
                if (jsonObject.contains(QStringLiteral("Actors")))
                    mMovieViewManager.setActors ( responseId,  jsonObject.value(QStringLiteral("Actors")).toString());
                if (jsonObject.contains(QStringLiteral("Plot")))
                    mMovieViewManager.setPlot (responseId,   jsonObject.value(QStringLiteral("Plot")).toString());
                if (jsonObject.contains(QStringLiteral("imdbRating")))
                    mMovieViewManager.setImdbRating (responseId,  jsonObject.value(QStringLiteral("imdbRating")).toString());
                if (jsonObject.contains (QStringLiteral("tomatoRating")))
                    mMovieViewManager.setTomatoRating ( responseId,  jsonObject.value(QStringLiteral("tomatoRating")).toString());
                if (jsonObject.contains(QStringLiteral("Website")))
                    mMovieViewManager.setWebsite ( responseId,  jsonObject.value(QStringLiteral("Website")).toString());
                if (jsonObject.contains(QStringLiteral("Poster")))
                    mMovieViewManager.setPoster (responseId,   jsonObject.value(QStringLiteral("Poster")).toString());
                if (jsonObject.contains(QStringLiteral("Genre")))
                    mMovieViewManager.setGenre ( responseId, jsonObject.value(QStringLiteral("Genre")).toString());
                success = true;
            }
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "OmdbResponseParser::run ecxception: " << e.what();
    }
    emit parsingComplete(responseId, success);
}
