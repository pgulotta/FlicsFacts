#include "omdbresponseparser.hpp"
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <exception>
#include <cassert>


static QString gDefaultGenre { QObject::tr("Not Defined")};
static QString gDefaultField { "        "};

static QHash<int, QString> mGenres
{
    {28, QObject::tr("Action")},
    {12, QObject::tr("Adventure")},
    {16, QObject::tr("Animation")},
    {35, QObject::tr("Comedy")},
    {80, QObject::tr("Crime")},
    {99, QObject::tr("Documentary")},
    {18, QObject::tr("Drama")},
    {10751, QObject::tr("Family")},
    {14, QObject::tr("Fantasy")},
    {36, QObject::tr("History")},
    {27, QObject::tr("Horror")},
    {10402, QObject::tr("Music")},
    {9648, QObject::tr("Mystery")},
    {10749, QObject::tr("Romance")},
    {878, QObject::tr("Science Fiction")},
    {10770, QObject::tr("TV Movie")},
    {53, QObject::tr("Thriller")},
    {10752, QObject::tr("War")},
    {37, QObject::tr("Western")}
};


static QString getGenres(const QJsonObject& jsonObject)
{
    QString result;
    if (jsonObject.contains(QStringLiteral("genre_ids")))
    {
        auto genreIds = jsonObject["genre_ids"];
        if ( genreIds.isArray() )
        {
            QJsonArray genreIdsArray = genreIds.toArray();
            if ( genreIdsArray.count() > 0)
            {
                for(auto genreId :genreIdsArray)
                {
                    bool addComma = !result.isEmpty();
                    auto cit =  mGenres.constFind(genreId.toInt());
                    if (cit != mGenres.cend())
                    {
                        result += (addComma)  ? ", " + cit.value()  :  cit.value();
                    }
                }
            }
        }
    }
    return result.isEmpty() ? gDefaultGenre : result;
}

OmdbResponseParser::OmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager) :
    QObject{parent},
    mMovieViewManager{movieViewManager}
{
}

void OmdbResponseParser::parseSearchResult( const QByteArray& source,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(QStringLiteral("results")))
                {
                    auto results = jsonObject["results"];
                    if ( results.isArray() )
                    {
                        QJsonArray resultsArray = results.toArray();
                        if ( resultsArray.count() > 0)
                        {
                            QJsonObject jsonObject = resultsArray.at(0).toObject();
                            if (jsonObject.contains(QStringLiteral("title")))
                            {
                                mMovieViewManager.setYear (responseId, gDefaultField);
                                mMovieViewManager.setRated (responseId, gDefaultField);
                                mMovieViewManager.setRuntime ( responseId, gDefaultField);
                                mMovieViewManager.setActors ( responseId, gDefaultField);

                                mMovieViewManager.setTitle (responseId, jsonObject.value(QStringLiteral("title")).toString());
                                if (jsonObject.contains(QStringLiteral("id")))
                                    mMovieViewManager.setMovieId(responseId,  jsonObject.value(QStringLiteral("id") ).toInt());
                                else
                                    mMovieViewManager.setMovieId(responseId, 0);
                                if (jsonObject.contains(QStringLiteral("release_date")))
                                    mMovieViewManager.setReleased ( responseId,  jsonObject.value(QStringLiteral("release_date")).toString());
                                else
                                    mMovieViewManager.setReleased ( responseId,  gDefaultField);
                                if (jsonObject.contains(QStringLiteral("overview")))
                                    mMovieViewManager.setPlot (responseId,   jsonObject.value(QStringLiteral("overview")).toString());
                                else
                                    mMovieViewManager.setPlot ( responseId, gDefaultField);
                                if (jsonObject.contains(QStringLiteral("popularity")))
                                    mMovieViewManager.setImdbRating (responseId,  jsonObject.value(QStringLiteral("popularity")).toString());
                                else
                                    mMovieViewManager.setImdbRating ( responseId, gDefaultField);
                                if (jsonObject.contains (QStringLiteral("vote_average")))
                                    mMovieViewManager.setTomatoRating ( responseId,  jsonObject.value(QStringLiteral("vote_average")).toString());
                                else
                                    mMovieViewManager.setTomatoRating ( responseId, gDefaultField);
                                if (jsonObject.contains(QStringLiteral("poster_path")))
                                    mMovieViewManager.setPoster (responseId, jsonObject.value(QStringLiteral("poster_path")).toString());
                                else
                                    mMovieViewManager.setPoster ( responseId, gDefaultField);
                                mMovieViewManager.setGenre ( responseId,  getGenres(jsonObject));

                                success = true;
                            }
                        }
                    }
                }
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseSearchResult ecxception: " << e.what();
        }
    }
    qDebug() << "OmdbResponseParser::parseSearchResult for responseId=" << responseId << " successful=" << success;
    emit searchParsingComplete(responseId, success);
}

void OmdbResponseParser::parseMovieDetails( const QByteArray& source,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(QStringLiteral("homepage")))
                    mMovieViewManager.setWebsite ( responseId,  jsonObject.value(QStringLiteral("homepage")).toString());
                else
                    mMovieViewManager.setWebsite ( responseId, gDefaultField);

                if (jsonObject.contains(QStringLiteral("runtime")))
                    mMovieViewManager.setRuntime ( responseId,  jsonObject.value(QStringLiteral("Runtime")).toString());
                else
                    mMovieViewManager.setRuntime ( responseId, gDefaultField);



//                        if (jsonObject.contains(QStringLiteral("Year")))
//                            mMovieViewManager.setYear (responseId,  jsonObject.value(QStringLiteral("Year")).toString());
//                        if (jsonObject.contains(QStringLiteral("Rated")))
//                            mMovieViewManager.setRated (responseId,  jsonObject.value(QStringLiteral("Rated")).toString());
//                        if (jsonObject.contains(QStringLiteral("Runtime")))
//                            mMovieViewManager.setRuntime ( responseId,  jsonObject.value(QStringLiteral("Runtime")).toString());
//                        if (jsonObject.contains(QStringLiteral("Actors")))
//                            mMovieViewManager.setActors ( responseId,  jsonObject.value(QStringLiteral("Actors")).toString());
//                        if (jsonObject.contains(QStringLiteral("Website")))
//                            mMovieViewManager.setWebsite ( responseId,  jsonObject.value(QStringLiteral("Website")).toString());
//                        if (jsonObject.contains(QStringLiteral("Genre")))
//                            mMovieViewManager.setGenre ( responseId, jsonObject.value(QStringLiteral("Genre")).toString());
                success = true;
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseGetDetails ecxception: " << e.what();
        }
    }
    qDebug() << "OmdbResponseParser::parseMovieDetails for responseId=" << responseId << " successful=" << success;
    emit detailsParsingComplete(responseId, success);
}
