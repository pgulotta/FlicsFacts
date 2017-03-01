#pragma once

#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
#include <../FlicsFacts/Model/movieresponse.hpp>
#include "../FlicsFacts/Controller/omdbresponseparser.hpp"
#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include "../FlicsFacts/Model/moviesearchresponse.hpp"
#include <QObject>
#include <QNetworkAccessManager>
#include <QFutureWatcher>

class QString;
class QtNetwork;
class QNetworkReply;
struct MovieResponse;


class MovieViewManager : public QObject
{
Q_OBJECT
Q_PROPERTY(QString requestFailed READ requestFailed CONSTANT )
Q_PROPERTY(QString appName READ appName CONSTANT )
Q_PROPERTY(QString appVersion READ appVersion CONSTANT )
Q_PROPERTY(QString appNameVersion READ appNameVersion CONSTANT )
Q_PROPERTY(QString titleRequest READ titleRequest NOTIFY titleRequestChanged)

public slots:
    void onNetworkReply(QNetworkReply *networkReply);
    void onSearchParsingComplete(int responseId,bool successful);
    void onDetailsParsingComplete(int responseId,bool successful);
    void onCreditsParsingComplete(int responseId,bool successful);
    void onShareResponsesFormatted();

signals:
    void responseReceived(int responseId);
    void titleRequestChanged(const QString&  titleRequest);
    void displayTextMessage(const QString&  title, QString message);

public:
    explicit MovieViewManager(QObject *parent = 0);
    explicit MovieViewManager(const MovieViewManager& rhs) = delete;
    MovieViewManager& operator= (const MovieViewManager& rhs) = delete;

    Q_INVOKABLE void shareMovieResponses();
    Q_INVOKABLE void findFlicSelected(const QString& movieTitle);
    Q_INVOKABLE void queryMovieSearch(int responseId, const QString& movieTitle);
    Q_INVOKABLE int removeSelectedMovie(int responseId);

    Q_INVOKABLE QString year(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Year;
    }

    Q_INVOKABLE QString rated(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated;
    }

    Q_INVOKABLE QString released(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Released;
    }

    Q_INVOKABLE QString runtime(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime;
    }

    Q_INVOKABLE QString actors(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors;
    }

    Q_INVOKABLE QString plot(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot;
    }

    Q_INVOKABLE QString popularity(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Popularity;
    }

    Q_INVOKABLE int movieId(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId;
    }

    Q_INVOKABLE QString languages(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Languages;
    }

    Q_INVOKABLE QString websiteUrl(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->WebsiteUrl;
    }
    Q_INVOKABLE QString website(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Website;
    }
    Q_INVOKABLE QString title(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Title;
    }

    Q_INVOKABLE QString poster(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Poster;
    }

    Q_INVOKABLE QString genre(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre;
    }

    Q_INVOKABLE QString status(int responseId) const
    {
        return mMovieResponses.at(static_cast<std::size_t>(responseId))->Status;
    }

    void setMovieId(int responseId, int movieId)
    {
        m_searchResponseModel.at(responseId)->setMovieId(movieId);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId = movieId;
    }

    void setStatus(int responseId, const QString& status)
    {
        m_searchResponseModel.at(responseId)->setStatus(status);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Status = status;
    }

    void setYear(int responseId, const QString&  year)
    {
        m_searchResponseModel.at(responseId)->setYear(year);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Year = year;
    }

    void setTitle(int responseId,const QString&  title)
    {
        m_searchResponseModel.at(responseId)->setTitle(title);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Title = title;
    }

    void setPoster(int responseId,const QString&  poster)
    {
        m_searchResponseModel.at(responseId)->setPoster(poster);
        mMovieResponses.at(static_cast<std::size_t>(responseId) )->Poster = poster;
    }

    void setGenre(int responseId, const QString&  genre)
    {
        m_searchResponseModel.at(responseId)->setGenre(genre);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre = genre;
    }

    void setPopularity(int responseId,const QString&  popularity)
    {
        m_searchResponseModel.at(responseId)->setPopularity(popularity);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Popularity = popularity;
    }

    void setLanguages(int responseId,const QString&  languages)
    {
        m_searchResponseModel.at(responseId)->setLanguages(languages);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Languages = languages;
    }

    void setWebsite(int responseId,const QString&  website)
    {
        m_searchResponseModel.at(responseId)->setWebsite(website);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Website = website;
    }

    void setWebsiteUrl(int responseId,const QString&  websiteUrl)
    {
        m_searchResponseModel.at(responseId)->setWebsiteUrl(websiteUrl);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->WebsiteUrl = websiteUrl;
    }

    void setRated(int responseId,const QString& rating)
    {
        m_searchResponseModel.at(responseId)->setRating(rating);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated = rating;
    }

    void setReleased(int responseId,const QString&  released)
    {
        m_searchResponseModel.at(responseId)->setReleased(released);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Released = released;
    }

    void setRuntime(int responseId,const QString&  runtime)
    {
        m_searchResponseModel.at(responseId)->setRuntime(runtime);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime = runtime;
    }

    void setActors(int responseId,const QString&  actors)
    {
        m_searchResponseModel.at(responseId)->setActors(actors);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors = actors;
    }
    void setPlot(int responseId,const QString&  plot)
    {
        m_searchResponseModel.at(responseId)->setPlot(plot);
        mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot = plot;
    }
    void setTitleRequest(const QString&  titleRequest)
    {
        m_titleRequest = titleRequest;
        emit titleRequestChanged(titleRequest);
    }

    QString appName() const
    {
        return m_appName;
    }

    QString appVersion() const
    {
        return m_appVersion;
    }

    QString appNameVersion() const
    {
        return appName() + " v" + appVersion();
    }

    QString titleRequest() const
    {
        return m_titleRequest;
    }

    QString requestFailed() const
    {
        return m_requestFailed;
    }

    QQmlObjectListModel<MovieSearchResponse>*  searchResponseModel()
    {
        return &m_searchResponseModel;
    }

private:
    void queryMovieDetails(int responseId, int movieId);
    void queryMovieCredits(int responseId, int movieId);

private:
    QString m_requestFailed;
    QString m_appName;
    QString m_appVersion;
    QString m_titleRequest;
    QNetworkAccessManager mNetworkAccessManager;
    std::vector <std::unique_ptr<MovieResponse> > mMovieResponses;
    ShareResponsesFormatter mShareResponsesFormatterformatter;
    QFutureWatcher<QString> mShareResponsesWatcher;
    OmdbResponseParser mOmdbResponseParser;
    QQmlObjectListModel<MovieSearchResponse> m_searchResponseModel;
};

