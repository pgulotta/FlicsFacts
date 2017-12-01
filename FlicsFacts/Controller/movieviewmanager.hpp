#pragma once

#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
#include "../FlicsFacts/Controller/networkrequestmovieattributes.hpp"
#include "../FlicsFacts/Controller/moviesearchparser.hpp"
#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include "../FlicsFacts/Model/movieresponse.hpp"
#include "../FlicsFacts/Model/sortedmovieresponsesmodel.hpp"
#include <QObject>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QFutureWatcher>


class QString;
class QtNetwork;
class QNetworkReply;
class QTimer;
class NetworkQueryDetails;


class MovieViewManager final : public QObject
{
Q_OBJECT
Q_PROPERTY(QString requestFailed READ requestFailed CONSTANT )
Q_PROPERTY(QString appName READ appName CONSTANT )
Q_PROPERTY(QString appVersion READ appVersion CONSTANT )
Q_PROPERTY(QString appNameVersion READ appNameVersion CONSTANT )
Q_PROPERTY(QString titleRequest READ titleRequest )

private slots:
    void onNetworkQueryTimer();
    void onNetworkReply(QNetworkReply *networkReply);
    void onMovieSearchParsingComplete(int responseId,bool successful);
    void onMovieDetailsParsingComplete(bool successful);
    void onMovieCreditsParsingComplete(bool successful);
    void onUpcomingMoviesParsingComplete( int responseId,bool successful);
    void onNowPlayingParsingComplete(int responseId,bool successful);
    void onShareResponsesFormatted();

public slots:
    void shareMovieResponses();
    void findFlicSelected(const QString& movieTitle);
    void tryQueryMovieSearch(int responseId);
    void removeSelectedMovie(int responseId);
    void removeAllMovieSearchResponses();
    void queryNowPlayingMovies();
    void queryUpcomongMovies();

signals:
    void responseReceived(int responseId);
    void displayTextMessage(const QString&  title, const QString& message) const;

public:
    explicit MovieViewManager(QObject *parent = 0);

//    virtual ~MovieViewManager() {
//        qDebug() << "~MovieViewManager() called";
//    }

    explicit MovieViewManager(const MovieViewManager& rhs) = delete;
    MovieViewManager& operator= (const MovieViewManager& rhs) = delete;



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
        return m_networkFailureMessage;
    }

    QQmlObjectListModel<MovieResponse>*  movieSearchResponses()
    {
        return &mMovieSearchResponses;
    }

    QQmlObjectListModel<MovieResponse>*  nowPlayingMoviesResponses()
    {
        return &mNowPlayingMoviesResponses;
    }

    QQmlObjectListModel<MovieResponse>*  upcomingMoviesResponses()
    {
        return &mUpcomingMoviesResponses;
    }

    SortedMovieResponsesModel*  sortedNowPlayingMoviesResponses()
    {
        return &mSortedNowPlayingMoviesResponses;
    }

    SortedMovieResponsesModel*  sortedUpcomingMoviesResponses()
    {
        return &mSortedUpcomingMoviesResponses;
    }

private:
    void queryMovieSearch(int responseId, const QString& movieTitle);
    void queryMovieDetails(int movieId, const QStringList& attributes);
    void queryMovieCredits(int movieId, const QStringList& attributes);
    void displayNothingToShare();
    void displayShareNotSupported();
    bool removeMovieSearchResponses();

private:
    QString m_networkFailureMessage;
    QString m_appName;
    QString m_appVersion;
    QString m_titleRequest;
    NetworkRequestMovieAttributes mMovieAttributes;
    QNetworkAccessManager mNetworkAccessManager;
    ShareResponsesFormatter mShareResponsesFormatterformatter;
    QFutureWatcher<QString> mShareResponsesWatcher;
    MovieSearchParser mResponseParser;
    QPointer<QTimer> mNetworkQueryTimer { new QTimer};
    QQmlObjectListModel<MovieResponse> mMovieSearchResponses;
    QQmlObjectListModel<MovieResponse> mNowPlayingMoviesResponses;
    QQmlObjectListModel<MovieResponse> mUpcomingMoviesResponses;
    QQueue<NetworkQueryDetails> mNetworkQueryDetailsQueue;
    SortedMovieResponsesModel mSortedNowPlayingMoviesResponses;
    SortedMovieResponsesModel mSortedUpcomingMoviesResponses;

};

struct NetworkQueryDetails
{
    int movieId;
    QStringList creditsAttributes;
    QStringList detailsAttributes;
};


