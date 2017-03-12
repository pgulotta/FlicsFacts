#include "moviesearchparser.hpp"
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
const QString homepageKey {"homepage"};
const QString runtimeKey {"runtime"};
const QString castKey {"cast"};
const QString nameKey {"name"};
const QString resultsKey {"results"};
const QString titleKey = "title";
const QString idKey = "id";




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
            QJsonArray genreIdsArray =genreIds.toArray();
            if ( genreIdsArray.count() > 0)
            {
                int counter = 0;
                for(auto genreId :genreIdsArray)
                {
                    bool addComma = !result.isEmpty();
                    auto cit =  mGenres.constFind(genreId.toInt());
                    if (cit != mGenres.cend())
                    {
                        result += (addComma)  ? ", " + cit.value()  :  cit.value();
                    }
                    if ( ++counter > 2)
                        break;
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
                int counter = 0;
                for(auto languageObject : languagesArray)
                {
                    bool addComma = !result.isEmpty();
                    auto languageName = languageObject.toObject().value(QStringLiteral("name")).toString();
                    result += (addComma)  ? ", " + languageName :  languageName;
                    if ( ++counter > 2)
                        break;
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

QString extractText(const QJsonObject& jsonObject,  const QString& key )
{
    if (jsonObject.contains(key))
    {
        QJsonValue value { jsonObject.value(key)};
        if (value !=QJsonValue::Null)
        {
            auto text = value.toString();
            if ( !text.isEmpty())
                return text;
        }
    }
    return nullptr;
}

QString extractText(const QJsonValue& value,  const QString& key )
{
    return (value == QJsonValue::Null) ? nullptr : extractText(value.toObject(), key);
}

QString extractInt(const QJsonObject& jsonObject,  const QString& key )
{
    if (jsonObject.contains(key))
    {
        QJsonValue value { jsonObject.value(key)};
        if (value !=QJsonValue::Null)
        {
            return QString::number(value.toInt());
        }
    }
    return nullptr;
}


MovieSearchParser::MovieSearchParser(QObject *parent) :
    QObject{parent}
{
}

void MovieSearchParser::parseMovieResponse(const QJsonObject &jsonObject, MovieResponse* movieResponse) const
{

    QString title = extractText(jsonObject, titleKey);
    if ( title  != nullptr)
    {
        movieResponse->setTitle ( title);
        movieResponse->setWebsite(  gDefaultField);
        movieResponse->setWebsiteUrl(  gDefaultField);
        movieResponse->setRuntime (  gDefaultField);
        movieResponse->setActors (  gDefaultField);
        movieResponse->setLanguages(   gDefaultField);

        QString id { extractInt(jsonObject, idKey)};
        if (id == nullptr)
            movieResponse->setMovieId( 0);
        else
            movieResponse->setMovieId( id.toInt());


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
    }
}

void MovieSearchParser::parseMovieSearchResult( const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(resultsKey))
                {
                    auto results = jsonObject["results"];
                    if ( results.isArray() )
                    {
                        QJsonArray resultsArray = results.toArray();
                        if ( resultsArray.count() > 0)
                        {
                            QJsonObject jsonObject = resultsArray.at(0).toObject();
                            auto movieResponse = destination.at(responseId);
                            if (movieResponse != nullptr)
                            {
                                parseMovieResponse(jsonObject, movieResponse);
                                success = movieResponse->movieId() > 0;
                            }
                        }
                    }
                }
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseSearchResult exception: " << e.what();
        }
    }
    emit movieSearchParsingComplete(responseId, success);
}

void MovieSearchParser::parseNowPlaying( const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(resultsKey))
                {
                    auto results = jsonObject["results"];
                    if ( results.isArray() )
                    {
                        QJsonArray resultsArray = results.toArray();
                        if ( resultsArray.count() >0)
                        {
                            success =  true;
                            destination.clear();
                            for(auto arrayItem : resultsArray)
                            {
                                destination.append( new MovieResponse( this));
                                responseId = destination.count()-1;
                                parseMovieResponse(arrayItem.toObject(), destination.at(responseId));
                                emit nowPlayingParsingComplete( responseId, success);
                            }

                        }
                    }
                }
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseSearchResult exception: " << e.what();
        }
    }
}

void MovieSearchParser::parseUpcomingMovies(const QByteArray &source, QQmlObjectListModel<MovieResponse>& destination,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document = QJsonDocument::fromJson(source);
            if ( !document.isNull() && !document.isEmpty())
            {
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(resultsKey))
                {
                    auto results = jsonObject["results"];
                    if ( results.isArray() )
                    {
                        QJsonArray resultsArray = results.toArray();
                        if ( resultsArray.count() >0)
                        {
                            success =  true;
                            destination.clear();
                            for(auto arrayItem : resultsArray)
                            {
                                destination.append( new MovieResponse( this));
                                responseId = destination.count()-1;
                                parseMovieResponse(arrayItem.toObject(), destination.at(responseId));
                                emit upcomingMoviesParsingComplete(responseId, success);
                            }
                        }
                    }
                }
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseSearchResult exception: " << e.what();
        }
    }
}

void MovieSearchParser::parseMovieDetails(const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination,  int responseId)
{
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document { QJsonDocument::fromJson(source)};
            if ( !document.isNull() && !document.isEmpty())
            {
                auto movieResponse = destination.at(responseId);
                QJsonObject jsonObject { document.object()};
                QString homepage {extractText(jsonObject, homepageKey)};
                if  (homepage  != nullptr)
                {
                    auto website {  QString("<a href=\"%1\">Go to ...</a>").arg(homepage)};
                    movieResponse->setWebsite ( website);
                    movieResponse->setWebsiteUrl ( homepage);
                }
                QString runtime {extractInt(jsonObject, runtimeKey)};
                if (runtime != nullptr)
                    movieResponse->setRuntime (QString("%1 min").arg(runtime));

                movieResponse->setLanguages( getLanguages(jsonObject));
                success = true;
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "MovieSearchParser::parseGetDetails exception: " << e.what();
        }
    }
    emit detailsParsingComplete(success);
}

void MovieSearchParser::parseMovieCredits(const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination, int responseId)
{
    QString result;
    bool success = false;
    if ( source != nullptr && source.size() >0)
    {
        try {
            QJsonDocument document { QJsonDocument::fromJson(source)};
            if ( !document.isNull() && !document.isEmpty())
            {
                auto movieResponse = destination.at(responseId);
                QJsonObject jsonObject = document.object();
                if (jsonObject.contains(castKey))
                {
                    QJsonValue cast { jsonObject.value(castKey)};
                    if ( cast.isArray() )
                    {
                        QJsonArray array = cast.toArray();
                        if ( array.count() > 0)
                        {
                            int actorsCount =1;
                            for(auto item :array)
                            {
                                auto actor = item.toObject().value(QStringLiteral("name")).toString();
                                result += (actorsCount > 1)  ? ", " + actor :  actor;
                                if ( ++actorsCount >8 )
                                    break;
                            }
                        }
                    }
                }
                if (!result.isNull())
                    movieResponse->setActors(result);
                success = true;
            }
        }
        catch(std::exception const & e)
        {
            qDebug() << "OmdbResponseParser::parseMovieCredits exception: " << e.what();
        }
    }
    emit creditsParsingComplete(success);
}
