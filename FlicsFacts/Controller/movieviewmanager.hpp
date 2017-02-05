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
    void onParsingComplete(int responseId,bool successful);
    void onShareResponsesFormatted();

signals:
    void requestCreated(QString tabTitle,  int responseId);
    void responseReceived(int responseId);
    void titleRequestChanged(QString titleRequest);
    void displayTextMessage(QString title, QString message);

public:
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
    Q_INVOKABLE void findFlicSelected(QString movieTitle);
    Q_INVOKABLE void queryMovieDetails(int responseId, QString movieTitle);
    Q_INVOKABLE int removeSelectedMovie(int responseId);


    explicit MovieViewManager(QObject *parent = 0);
    explicit MovieViewManager(const MovieViewManager& rhs) = delete;
    MovieViewManager& operator= (const MovieViewManager& rhs) = delete;
    void setYear(int responseId, QString year);
    void setTitle(int responseId,QString title);
    void setPoster(int responseId,QString poster);
    void setGenre(int responseId, QString genre);
    void setImdbRating(int responseId,QString imdbRating);
    void setTomatoRating(int responseId,QString tomatoRating);
    void setWebsite(int responseId,QString website);
    void setRated(int responseId,QString rated);
    void setReleased(int responseId,QString released);
    void setRuntime(int responseId,QString runtime);
    void setActors(int responseId,QString actors);
    void setPlot(int responseId,QString plot);
    void setStatus(int responseId,QString status);
    void setTitleRequest(QString titleRequest);

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
        return appName() + " " + appVersion();
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
    QString formatUrl(QString movieTitle);

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

