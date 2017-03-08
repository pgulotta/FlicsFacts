#include "movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareclient.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QtGui/QTouchDevice>
#include <QApplication>
#include <QString>
#include <QtNetwork>
#include <QDebug>

const QString gAppName {"FlicsFacts"};
const QString gSearchRequest { "http://api.themoviedb.org/3/search/movie?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US&query=%1&page=1&include_adult=false"};
const QString gDetailsRequest {"http://api.themoviedb.org/3/movie/%1?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US"};
const QString gCreditsRequest {"http://api.themoviedb.org/3/movie/%1/credits?api_key=2839bfb130659459d7d9972ad9aa3cd4"};

QString formatMovieSearchUrl(const QString& movieTitle)
{
    return (QString ( gSearchRequest.arg(movieTitle.trimmed().replace(' ','+') )  ) );;
}

QString formatMovieDetailsUrl(int movieId)
{
    return (QString ( gDetailsRequest.arg(movieId) ) );
}

QString formatMovieCreditsUrl(int movieId)
{
    return (QString ( gCreditsRequest.arg(movieId) ) );
}

MovieViewManager::MovieViewManager(QObject *parent) :
    QObject{parent},
    m_networkFailureMessage{tr("A network issue occurred when querying \nmovie")},
    m_appName{gAppName},
    m_appVersion{"1.11"},
    mShareResponsesFormatterformatter{parent},
    mShareResponsesWatcher{parent},
    mTmdbResponseParser{parent, *this},
    m_searchResponseModel { parent}
{
    connect(&mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MovieViewManager::onNetworkReply);
    connect(&mShareResponsesWatcher, &QFutureWatcher<QString>::finished, this, &MovieViewManager::onShareResponsesFormatted);
    connect(&mTmdbResponseParser, &TmdbResponseParser::searchParsingComplete, this,  &MovieViewManager::onSearchParsingComplete);
    connect(&mTmdbResponseParser, &TmdbResponseParser::detailsParsingComplete, this,  &MovieViewManager::onDetailsParsingComplete);
    connect(&mTmdbResponseParser, &TmdbResponseParser::creditsParsingComplete, this,  &MovieViewManager::onCreditsParsingComplete);
}

void MovieViewManager::findFlicSelected(const QString& movieTitle)
{
    if (movieTitle.isEmpty())
        return;

    int responseIndex = m_searchResponseModel.count();
    m_searchResponseModel.append( new MovieSearchResponse(movieTitle, this));
    queryMovieSearch(responseIndex, movieTitle);
}

void MovieViewManager::removeSelectedMovie(int responseId)
{
    if ( responseId <0 ||  responseId >=  m_searchResponseModel.count() )
        return;
    auto movieResponse = m_searchResponseModel.at(responseId);
    m_searchResponseModel.remove(responseId);
    movieResponse->setParent(nullptr);
    delete  movieResponse;
}

void MovieViewManager::removeAllMovieSearchResponses()
{
    try
    {
        while (m_searchResponseModel.count() > 0)
        {
            removeSelectedMovie(m_searchResponseModel.count() - 1);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::removeAllMovieSearchResponses ecxception: " << e.what();
    }
}

bool MovieViewManager::removeMovieSearchResponses()
{
    bool result = false;
    try
    {
        while (m_searchResponseModel.count() > 0)
        {
            removeSelectedMovie(m_searchResponseModel.count() - 1);
        }
        result = true;
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::removeAllMovieSearchResponses ecxception: " << e.what();
    }
    return result;
}

void MovieViewManager::shareMovieResponses()
{
#ifdef Q_OS_ANDROID
    if (m_searchResponseModel.count() == 0)
    {
        displayNothingToShare();
    }
    else
    {
        QFuture<QString> future = QtConcurrent::run<QString>(&mShareResponsesFormatterformatter, &ShareResponsesFormatter::formatAsText,
                                                             m_searchResponseModel.constBegin(), m_searchResponseModel.constEnd());
        mShareResponsesWatcher.setFuture(future);
    }
#else
    displayShareNotSupported();
#endif
}

void MovieViewManager::queryMovieCredits(int responseId, int movieId)
{
    auto request = QNetworkRequest( formatMovieCreditsUrl(movieId));
    QStringList attributes {QString::number(responseId), QString::number(movieId), QStringLiteral("Credits")};
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
    mNetworkAccessManager.get(request);
}

void MovieViewManager::queryMovieDetails(int responseId, int movieId)
{
    auto request = QNetworkRequest( formatMovieDetailsUrl(movieId));
    QStringList attributes {QString::number(responseId), QString::number(movieId)};
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
    mNetworkAccessManager.get(request);
}

void MovieViewManager::tryQueryMovieSearch(int responseId)
{
    if ( responseId <0 ||  responseId >=  m_searchResponseModel.count() )
        return;
    auto searchResponse = m_searchResponseModel.at(responseId);
    if (searchResponse->status().startsWith(m_networkFailureMessage))
    {
        queryMovieSearch(responseId, searchResponse->title());
    }
}

void MovieViewManager::queryMovieSearch(int responseId,const QString& movieTitle)
{
    if ( movieTitle.isEmpty())
        return;
    auto request = QNetworkRequest( formatMovieSearchUrl(movieTitle));
    QStringList attributes {QString::number( responseId)};
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
    mNetworkAccessManager.get(request);
}
void MovieViewManager::onNetworkReply(QNetworkReply *networkReply)
{
    QStringList attributes = networkReply->request().attribute(QNetworkRequest::Attribute::User).toStringList();
    int responseId = attributes.at(0).toInt();
    if ( networkReply->error())
    {
        auto errorMessage = networkReply->errorString().length() > 50    ? "" : networkReply->errorString();
        auto searchResponse = m_searchResponseModel.at(responseId);
        searchResponse->setStatus ( QString ("%1 %2.\n%3").arg(m_networkFailureMessage).arg(searchResponse->title()).arg( errorMessage));
        emit responseReceived(responseId);
    }
    else
    {
        const QByteArray source = networkReply->readAll();
        if ( attributes.count()==3)
        {
            mTmdbResponseParser.parseMovieCredits(source, responseId);
        }
        else
        if ( attributes.count()==2)
        {
            mTmdbResponseParser.parseMovieDetails(source, responseId);
        }
        else
        {
            mTmdbResponseParser.parseSearchResult(source, responseId);
        }
    }
    networkReply->deleteLater();
}

void MovieViewManager::onSearchParsingComplete(int responseId, bool successful)
{
    auto movieResponse = m_searchResponseModel.at(responseId);
    if (successful)
    {
        m_searchResponseModel.at(responseId)->setStatus ("");
        queryMovieDetails (responseId, movieResponse->movieId());
        queryMovieCredits(responseId, movieResponse->movieId());
    }
    else
    {
        movieResponse->setStatus(tr("Unable to find the selected flic."));
    }
    emit responseReceived(responseId);
}

void MovieViewManager::onDetailsParsingComplete(int responseId, bool successful)
{
    if (successful)
    {
        emit responseReceived(responseId);
    }
}

void MovieViewManager::onCreditsParsingComplete(int responseId, bool successful)
{
    if (successful)
    {
        emit responseReceived(responseId);
    }
}

void MovieViewManager::displayNothingToShare()
{

    emit displayTextMessage( tr( "Share FlicsFacts"), tr("There is nothing to share."));
}

void MovieViewManager::displayShareNotSupported()
{

    emit displayTextMessage( tr( "Share FlicsFacts"), tr("Sorry, sharing is not available."));
}

void MovieViewManager::onShareResponsesFormatted()
{
    if ( mShareResponsesWatcher.result().size() == 0)
    {
        displayNothingToShare();
    }
    else
    {
        ShareClient shareClient(parent());
        shareClient.setShare(mShareResponsesWatcher.result());
    }
}




























