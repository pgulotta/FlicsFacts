#pragma once

#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
#include <../FlicsFacts/Model/movieresponse.hpp>
#include "../FlicsFacts/Controller/omdbresponseparser.hpp"
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
    void onShareResponsesFormatted();

signals:
    void requestCreated(const QString&  tabTitle,  int responseId);
    void responseReceived(int responseId);
    void titleRequestChanged(const QString&  titleRequest);
    void displayTextMessage(const QString&  title, QString message);

public:
    Q_INVOKABLE int movieId(int responseId) const;
    Q_INVOKABLE QString year(int responseId) const;
    Q_INVOKABLE QString rated(int responseId) const;
    Q_INVOKABLE QString released(int responseId) const;
    Q_INVOKABLE QString runtime(int responseId) const;
    Q_INVOKABLE QString actors(int responseId) const;
    Q_INVOKABLE QString plot(int responseId) const;
    Q_INVOKABLE QString imdbRating(int responseId) const;
    Q_INVOKABLE QString website(int responseId) const;
    Q_INVOKABLE QString title(int responseId) const;
    Q_INVOKABLE QString poster(int responseId) const;
    Q_INVOKABLE QString genre(int responseId) const;
    Q_INVOKABLE QString tomatoRating(int responseId) const;
    Q_INVOKABLE QString status(int responseId) const;
    Q_INVOKABLE void shareMovieResponses();
    Q_INVOKABLE void findFlicSelected(const QString& movieTitle);
    Q_INVOKABLE void queryMovieSearch(int responseId, const QString& movieTitle);
    Q_INVOKABLE void queryMovieDetails(int responseId, int movieId);
    Q_INVOKABLE int removeSelectedMovie(int responseId);


    explicit MovieViewManager(QObject *parent = 0);
    explicit MovieViewManager(const MovieViewManager& rhs) = delete;
    MovieViewManager& operator= (const MovieViewManager& rhs) = delete;
    void setMovieId(int responseId, int movieId);
    void setYear(int responseId, const QString&  year);
    void setTitle(int responseId,const QString&  title);
    void setPoster(int responseId,const QString&  poster);
    void setGenre(int responseId, const QString&  genre);
    void setImdbRating(int responseId,const QString&  imdbRating);
    void setTomatoRating(int responseId,const QString&  tomatoRating);
    void setWebsite(int responseId,const QString&  website);
    void setRated(int responseId,const QString&  rated);
    void setReleased(int responseId,const QString&  released);
    void setRuntime(int responseId,const QString&  runtime);
    void setActors(int responseId,const QString&  actors);
    void setPlot(int responseId,const QString&  plot);
    void setStatus(int responseId,const QString& status);
    void setTitleRequest(const QString&  titleRequest);

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

private:
    QString formatMovieSearchUrl(const QString& movieTitle);
    QString formatMovieDetailsUrl(int movieId);

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

};

