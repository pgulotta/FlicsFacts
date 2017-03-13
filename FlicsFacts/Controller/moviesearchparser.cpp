#include "moviesearchparser.hpp"
#include "../FlicsFacts/Controller/movieviewmanager.hpp"
#include "../FlicsFacts/Model/movieresponse.hpp"
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
const QString titleKey { "title"};
const QString idKey { "id"};
const QString genreIdsKey {"genre_ids"};
const QString spokenLanguagesKey {"spoken_languages"};
const QString releaseDateKey {"release_date"};
const QString overviewKey { "overview"};
const QString popularityKey { "popularity"};
const QString voteAverageKey { "vote_average"};
const QString posterPathKey { "poster_path"};


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


static QString convertJsonDoubleToString(const QJsonObject& jsonObject, const QString& key  )
{
    if (jsonObject.contains(key))
    {
        QJsonValue value { jsonObject.value(key)};
        return (value  == QJsonValue::Null) ? nullptr : QString::number(value.toDouble());
    }
    else
        return nullptr;
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

static QString getLanguages(const QJsonObject& jsonObject)
{
    QString result;
    if (jsonObject.contains(spokenLanguagesKey))
    {
        QJsonValue languages { jsonObject.value(spokenLanguagesKey)};
        if ( languages.isArray() )
        {
            QJsonArray languagesArray = languages.toArray();
            if ( languagesArray.count() > 0)
            {
                int counter = 0;
                QString languageName;
                for(auto languageObject : languagesArray)
                {
                    bool addComma = !result.isEmpty();
                    languageName  = extractText(languageObject, nameKey);
                    if (languageName != nullptr)
                    {
                        result += (addComma)  ? ", " + languageName :  languageName;
                        if ( ++counter > 2)
                            break;
                    }
                }
            }
        }
    }
    return result.isEmpty() ? gNotDefined : result;
}

static QString getGenres(const QJsonObject& jsonObject)
{
    QString result;
    if (jsonObject.contains(genreIdsKey))
    {
        QJsonValue genreIds { jsonObject.value(genreIdsKey)};
        if ( genreIds.isArray() )
        {
            QJsonArray genreIdsArray =genreIds.toArray();
            if ( genreIdsArray.count() > 0)
            {
                int counter = 0;
                for(auto genreId : genreIdsArray)
                {
                    bool addComma = !result.isEmpty();
                    auto cit = mGenres.constFind(genreId.toInt());
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
            movieResponse->setMovieId(MovieResponse::invalidMovieId());
        else
            movieResponse->setMovieId( id.toInt());

        QString releaseDate {extractText(jsonObject, releaseDateKey)};
        if (releaseDate == nullptr)
        {
            movieResponse->setReleased (gDefaultField);
            movieResponse->setYear (gDefaultField);
        }
        else
        {
            movieResponse->setReleased ( releaseDate);
            movieResponse->setYear (  extractYear(releaseDate) );
        }

        QString overview {extractText(jsonObject, overviewKey)};
        if (overview == nullptr)
            movieResponse->setPlot (  gDefaultField);
        else
            movieResponse->setPlot ( overview);

        QString popularity {convertJsonDoubleToString (jsonObject,popularityKey)};
        if (popularity == nullptr)
            movieResponse->setPopularity ( gDefaultField);
        else
            movieResponse->setPopularity (popularity);

        QString voterAverage {convertJsonDoubleToString (jsonObject,voteAverageKey)};
        if (voterAverage == nullptr)
            movieResponse->setRating ( gDefaultField);
        else
            movieResponse->setRating (voterAverage);

        QString posterPath {convertJsonDoubleToString (jsonObject,posterPathKey)};
        if (posterPath == nullptr)
            movieResponse->setPoster ( gDefaultField);
        else
            movieResponse->setPoster (posterPath);

        movieResponse->setGenre ( getGenres(jsonObject));
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
                    auto results = jsonObject[resultsKey];
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
                    auto results = jsonObject[resultsKey];
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
                QJsonObject jsonObject { document.object()};
                if (jsonObject.contains(resultsKey))
                {
                    QJsonValue results {jsonObject[resultsKey]};
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
                    QString website {  QString("<a href=\"%1\">Go to ...</a>").arg(homepage)};
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
                            QString actor;
                            for(auto item :array)
                            {
                                actor = extractText(item, nameKey);
                                if (actor != nullptr)
                                {
                                    result += (actorsCount > 1)  ? ", " + actor :  actor;
                                    if ( ++actorsCount >8 )
                                        break;
                                }
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
