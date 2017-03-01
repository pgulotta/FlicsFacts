#pragma once

#include "../FlicsFacts/Controller/shareresponsesformatter.hpp"
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
    ShareResponsesFormatter mShareResponsesFormatterformatter;
    QFutureWatcher<QString> mShareResponsesWatcher;
    OmdbResponseParser mOmdbResponseParser;
    QQmlObjectListModel<MovieSearchResponse> m_searchResponseModel;
};

