#include "tmdbresponseparser.hpp"
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QHash>
#include <exception>
#include <cassert>


static QString gNotDefined { QObject::tr("Not Defined")};
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
    return result.isEmpty() ? gNotDefined : result;
}

static QString getLanguages(const QJsonObject& jsonObject)
{
    QString result;
    if (jsonObject.contains(QStringLiteral("spoken_languages")))
    {
        auto languages = jsonObject["spoken_languages"];
        if ( languages.isArray() )
        {
            QJsonArray languagesArray = languages.toArray();
            if ( languagesArray.count() > 0)
            {
                for(auto languageObject :languagesArray)
                {
                    bool addComma = !result.isEmpty();
                    auto languageName = languageObject.toObject().value(QStringLiteral("name")).toString();
                    result += (addComma)  ? ", " + languageName :  languageName;
                }
            }
        }
    }
    return result.isEmpty() ? gNotDefined : result;
}

static QString convertJsonDoubleToString( const QString& key, const QJsonObject& source )
{
    return QString("%1").arg(source.value(key).toDouble());
}

static QString extractYear( const QString& source)
{
    return source.mid(0,4);
}

TmdbResponseParser::TmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager) :
    QObject{parent},
    mMovieViewManager{movieViewManager}
{
}

void TmdbResponseParser::parseSearchResult( const QByteArray& source,  int responseId)
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
                            auto movieResponse = mMovieViewManager.searchResponseModel()->at(responseId);
                            QJsonObject jsonObject = resultsArray.at(0).toObject();
                            if (jsonObject.contains(QStringLiteral("title")))
                            {
                                movieResponse->setWebsite(  gDefaultField);
                                movieResponse->setWebsiteUrl(  gDefaultField);
                                movieResponse->setRuntime (  gDefaultField);
                                movieResponse->setActors (  gDefaultField);
                                movieResponse->setLanguages(   gDefaultField);
                                movieResponse->setTitle ( jsonObject.value(QStringLiteral("title")).toString());

                                if (jsonObject.contains(QStringLiteral("id")))
                                    movieResponse->setMovieId(  jsonObject.value(QStringLiteral("id") ).toInt());
                                else
                                    movieResponse->setMovieId( 0);

                                if (jsonObject.contains(QStringLiteral("release_date")))
                                {
                                    QString releaseDate = jsonObject.value(QStringLiteral("release_date")).toString();
                                    movieResponse->setReleased (  releaseDate);
                                    movieResponse->setYear (  extractYear(releaseDate) );
                                }
                                else
                                    movieResponse->setReleased (   gDefaultField);

                                if (jsonObject.contains(QStringLiteral("overview")))
                                    movieResponse->setPlot (   jsonObject.value(QStringLiteral("overview")).toString());
                                else
                                    movieResponse->setPlot (  gDefaultField);

                                if (jsonObject.contains(QStringLiteral("popularity")))
                                    movieResponse->setPopularity ( convertJsonDoubleToString (QStringLiteral("popularity"), jsonObject));
                                else
                                    movieResponse->setPopularity (  gDefaultField);

                                if (jsonObject.contains (QStringLiteral("vote_average")))
                                    movieResponse->setRating(convertJsonDoubleToString(QStringLiteral("vote_average"), jsonObject));
                                else
                                    movieResponse->setRating( gDefaultField);
                                if (jsonObject.contains(QStringLiteral("poster_path")))
                                    movieResponse->setPoster ( jsonObject.value(QStringLiteral("poster_path")).toString());
                                else
                                    movieResponse->setPoster (  gDefaultField);
                                movieResponse->setGenre (   getGenres(jsonObject));

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
    emit searchParsingComplete(responseId, success);
}

void TmdbResponseParser::parseMovieDetails( const QByteArray& source,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                auto movieResponse = mMovieViewManager.searchResponseModel()->at(responseId);
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(QStringLiteral("homepage")))
                {
                    auto homepage = jsonObject.value(QStringLiteral("homepage")).toString();
                    if ( !homepage.isEmpty() )
                    {
                        auto website = "<a href=\"" + jsonObject.value(QStringLiteral("homepage")).toString() + "\">Go to ...</a>";
                        movieResponse->setWebsite (   website);
                        movieResponse->setWebsiteUrl (   homepage);
                    }
                }
                if (jsonObject.contains(QStringLiteral("runtime")))
                    movieResponse->setRuntime (   QString("%1 min").arg(jsonObject.value(QStringLiteral("runtime")).toInt()));

                movieResponse->setLanguages(   getLanguages(jsonObject));
                success = true;
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseGetDetails ecxception: " << e.what();
        }
    }
    emit detailsParsingComplete(responseId, success);
}

void TmdbResponseParser::parseMovieCredits( const QByteArray& source,  int responseId)
{
    QString result;
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                auto movieResponse = mMovieViewManager.searchResponseModel()->at(responseId);
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(QStringLiteral("cast")))
                {
                    auto cast = jsonObject["cast"];
                    if ( cast.isArray() )
                    {
                        QJsonArray array = cast.toArray();
                        if ( array.count() > 0)
                        {
                            int actorsCount =1;
                            for(auto item :array)
                            {
                                auto languageName = item.toObject().value(QStringLiteral("name")).toString();
                                result += (actorsCount > 1)  ? ", " + languageName :  languageName;
                                if ( ++actorsCount >8 )
                                    break;
                            }
                        }
                    }
                }
                movieResponse->setActors(  result);
                success = true;
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseMovieCredits ecxception: " << e.what();
        }
    }
    emit creditsParsingComplete(responseId, success);
}
