#include "movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareclient.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QtGui/QTouchDevice>
#include <QApplication>
#include <QString>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>


const QString gAppName {"FlicsFacts"};

const int gQueryTimerIntervalMs         {500};
const QString gMovieSearchRequest       {"http://api.themoviedb.org/3/search/movie?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US&query=%1&page=1&include_adult=false"};
const QString gMovieDetailsRequest      {"http://api.themoviedb.org/3/movie/%1?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US"};
const QString gMovieCreditsRequest      {"http://api.themoviedb.org/3/movie/%1/credits?api_key=2839bfb130659459d7d9972ad9aa3cd4"};
const QString gUpcomongMoviesRequest    {"http://api.themoviedb.org/3/movie/upcoming?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US&page=1"};
const QString gNowPlayingMoviesRequest  {"http://api.themoviedb.org/3/movie/now_playing?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US&page=1"};


QString formatMovieSearchUrl(const QString& movieTitle)
{
    return (QString ( gMovieSearchRequest.arg(movieTitle.trimmed().replace(' ','+') )  ) );;
}

QString formatMovieDetailsUrl(int movieId)
{
    return (QString ( gMovieDetailsRequest.arg(movieId) ) );
}

QString formatMovieCreditsUrl(int movieId)
{
    return (QString ( gMovieCreditsRequest.arg(movieId) ) );
}

MovieViewManager::MovieViewManager(QObject *parent) :
    QObject{parent},
    m_networkFailureMessage{tr("Unable to access the internet at this time.\nYou may want to try again shortly.")},
    m_appName{gAppName},
    m_appVersion{"1.11"},
    mShareResponsesFormatterformatter{parent},
    mShareResponsesWatcher{parent},
    mResponseParser{parent},
    mNetworkQueryTimer{ new QTimer {
        parent
    }},
    mMovieSearchResponses { parent},mNowPlayingMoviesResponses {parent},mUpcomingMoviesResponses { parent},
mSortedNowPlayingMoviesResponses { &mNowPlayingMoviesResponses, parent},
mSortedUpcomingMoviesResponses { &mUpcomingMoviesResponses, parent}

{
    connect(&mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MovieViewManager::onNetworkReply);
    connect(&mShareResponsesWatcher, &QFutureWatcher<QString>::finished, this, &MovieViewManager::onShareResponsesFormatted);
    connect(&mResponseParser, &MovieSearchParser::movieSearchParsingComplete, this,  &MovieViewManager::onMovieSearchParsingComplete);
    connect(&mResponseParser, &MovieSearchParser::nowPlayingParsingComplete, this,  &MovieViewManager::onNowPlayingParsingComplete);
    connect(&mResponseParser, &MovieSearchParser::upcomingMoviesParsingComplete, this,  &MovieViewManager::onUpcomingMoviesParsingComplete);
    connect(&mResponseParser, &MovieSearchParser::detailsParsingComplete, this,  &MovieViewManager::onMovieDetailsParsingComplete);
    connect(&mResponseParser, &MovieSearchParser::creditsParsingComplete, this,  &MovieViewManager::onMovieCreditsParsingComplete);
    connect(mNetworkQueryTimer, &QTimer::timeout, this, &MovieViewManager::onNetworkQueryTimer);
}


void MovieViewManager::findFlicSelected(const QString& movieTitle)
{
    try
    {
        if (!movieTitle.isEmpty())
        {
            int responseIndex = mMovieSearchResponses.count();
            mMovieSearchResponses.append( new MovieResponse(movieTitle, this));
            queryMovieSearch(responseIndex, movieTitle);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::findFlicSelected exception: " << e.what();
    }
}

void MovieViewManager::removeSelectedMovie(int responseId)
{
    try
    {
        if ( responseId <0 ||  responseId >=  mMovieSearchResponses.count() )
            return;
        auto movieResponse = mMovieSearchResponses.at(responseId);
        mMovieSearchResponses.remove(responseId);
        movieResponse->setParent(nullptr);
        delete  movieResponse;
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::removeSelectedMovie exception: " << e.what();
    }
}

void MovieViewManager::removeAllMovieSearchResponses()
{
    try
    {
        while (mMovieSearchResponses.count() > 0)
        {
            removeSelectedMovie(mMovieSearchResponses.count() - 1);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::removeAllMovieSearchResponses exception: " << e.what();
    }
}

bool MovieViewManager::removeMovieSearchResponses()
{
    bool result = false;
    try
    {
        while (mMovieSearchResponses.count() > 0)
        {
            removeSelectedMovie(mMovieSearchResponses.count() - 1);
        }
        result = true;
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::removeAllMovieSearchResponses exception: " << e.what();
    }
    return result;
}

void MovieViewManager::shareMovieResponses()
{
    try
    {
#ifdef Q_OS_ANDROID
        if (mMovieSearchResponses.count() == 0)
        {
            displayNothingToShare();
        }
        else
        {
            QFuture<QString> future = QtConcurrent::run<QString>(&mShareResponsesFormatterformatter, &ShareResponsesFormatter::formatAsText,
                                                                 mMovieSearchResponses.constBegin(), mMovieSearchResponses.constEnd());
            mShareResponsesWatcher.setFuture(future);
        }
#else
        displayShareNotSupported();
#endif
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::shareMovieResponses exception: " << e.what();
    }
}

void MovieViewManager::tryQueryMovieSearch(int responseId)
{
    try
    {
        if ( responseId <0 ||  responseId >=  mMovieSearchResponses.count() )
            return;
        auto searchResponse = mMovieSearchResponses.at(responseId);
        if (searchResponse->status().startsWith(m_networkFailureMessage))
        {
            queryMovieSearch(responseId, searchResponse->title());
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::tryQueryMovieSearch exception: " << e.what();
    }
}

void MovieViewManager::queryMovieSearch(int responseId,const QString& movieTitle)
{
    try
    {
        if ( !movieTitle.isEmpty())
        {
            auto request = QNetworkRequest( formatMovieSearchUrl(movieTitle));
            QStringList attributes;
            mMovieAttributes.assignMovieSearchAttributes( responseId, attributes);
            request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
            mNetworkAccessManager.get(request);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::queryMovieSearch exception: " << e.what();
    }
}

void MovieViewManager::queryMovieCredits(int movieId, const QStringList& attributes)
{
    try
    {
        auto request = QNetworkRequest( formatMovieCreditsUrl(movieId));
        request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
        mNetworkAccessManager.get(request);
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::queryMovieCredits exception: " << e.what();
    }
}

void MovieViewManager::queryMovieDetails(int movieId, const QStringList& attributes)
{
    try
    {
        auto request = QNetworkRequest( formatMovieDetailsUrl(movieId));
        request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
        mNetworkAccessManager.get(request);
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::queryMovieDetails exception: " << e.what();
    }
}

void MovieViewManager::queryUpcomongMovies()
{
    try
    {
        if (mUpcomingMoviesResponses.count() > 0)
        {
            auto searchResponse = mUpcomingMoviesResponses.at(0);
            if (searchResponse->status().startsWith(m_networkFailureMessage))
            {
                mUpcomingMoviesResponses.clear();
            }
        }

        if (mUpcomingMoviesResponses.count() == 0)
        {
            QStringList attributes;
            mMovieAttributes.assignUpcomingMoviesAttributes( 0,attributes);
            mUpcomingMoviesResponses.append( new MovieResponse( this));
            auto request = QNetworkRequest(gUpcomongMoviesRequest);
            request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
            mNetworkAccessManager.get(request);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::queryUpcomongMovies exception: " << e.what();
    }
}

void MovieViewManager::queryNowPlayingMovies()
{
    try
    {
        if (mNowPlayingMoviesResponses.count() >0)
        {
            auto movieResponse = mNowPlayingMoviesResponses.at(0);
            if (movieResponse->status().startsWith(m_networkFailureMessage))
            {
                mNowPlayingMoviesResponses.clear();
            }
        }

        if (mNowPlayingMoviesResponses.count() == 0)
        {
            QStringList attributes;
            mMovieAttributes.assignMoviesNowPlayingAttributes( 0, attributes);
            mNowPlayingMoviesResponses.append( new MovieResponse( this));
            auto request = QNetworkRequest(gNowPlayingMoviesRequest);
            request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
            mNetworkAccessManager.get(request);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::queryNowPlayingMovies exception: " << e.what();
    }
}

void MovieViewManager::onNetworkReply(QNetworkReply *networkReply)
{
    if ( networkReply == nullptr)
        return;
    try
    {
        QStringList attributes = networkReply->request().attribute(QNetworkRequest::Attribute::User).toStringList();
        queryType typeOfQuery = mMovieAttributes.extractQueryTypeAttributes(attributes);
        if ( typeOfQuery == queryType::undefined )
        {
            qDebug() << "MovieViewManager::onNetworkReply: Unable to identify type of network query.";
        }
        else
        {
            int responseId = mMovieAttributes.extractResourceIdAttributes(attributes);
            if ( networkReply->error())
            {
                auto errorMessage = networkReply->errorString().length() > 50    ? "" : networkReply->errorString();
                MovieResponse* searchResponse = nullptr;
                if ( typeOfQuery== queryType::moviesNowPlaying)
                    searchResponse = mNowPlayingMoviesResponses.at(responseId);
                else if ( typeOfQuery== queryType::upcomingMovies)
                    searchResponse = mUpcomingMoviesResponses.at(responseId);
                else
                    searchResponse = mMovieSearchResponses.at(responseId);
                searchResponse->setStatus ( QString ("%1\n%2").arg(m_networkFailureMessage).arg( errorMessage));
                emit responseReceived(responseId);
            }
            else
            {
                const QByteArray source = networkReply->readAll();
                switch(typeOfQuery)
                {
                case queryType::movieSearch:
                    mResponseParser.parseMovieSearchResult(source, mMovieSearchResponses,  responseId);
                    break;
                case queryType::movieSearchDetails:
                    mResponseParser.parseMovieDetails( source, mMovieSearchResponses, responseId);
                    break;
                case queryType::movieSearchCredits:
                    mResponseParser.parseMovieCredits(source, mMovieSearchResponses, responseId);
                    break;
                case queryType::moviesNowPlaying:
                    mResponseParser.parseNowPlaying( source, mNowPlayingMoviesResponses, responseId);
                    break;
                case queryType::moviesNowPlayingDetails:
                    mResponseParser.parseMovieDetails(source, mNowPlayingMoviesResponses, responseId);
                    break;
                case queryType::moviesNowPlayingCredits:
                    mResponseParser.parseMovieCredits(source, mNowPlayingMoviesResponses, responseId);
                    break;
                case queryType::upcomingMovies:
                    mResponseParser.parseUpcomingMovies( source, mUpcomingMoviesResponses, responseId);
                    break;
                case queryType::upcomingMoviesDetails:
                    mResponseParser.parseMovieDetails(source, mUpcomingMoviesResponses, responseId);
                    break;
                case queryType::upcomingMoviesCredits:
                    mResponseParser.parseMovieCredits(source, mUpcomingMoviesResponses, responseId);
                    break;
                case queryType::undefined:
                default:
                    qDebug() << "MovieViewManager::onNetworkReply: Unable to parse netork response ";
                    break;
                }
            }
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onNetworkReply exception: " << e.what();
    }
    networkReply->deleteLater();
}

void MovieViewManager::onMovieSearchParsingComplete(int responseId, bool successful)
{
    try
    {
        auto movieResponse = mMovieSearchResponses.at(responseId);
        if (successful)
        {
            movieResponse->setStatus ("");
            QStringList attributes;
            mMovieAttributes.assignMovieSearchDetailsAttributes(responseId, movieResponse->movieId(), attributes);
            queryMovieDetails (movieResponse->movieId(),attributes );
            mMovieAttributes.assignMovieSearchCreditsAttributes(responseId, movieResponse->movieId(), attributes);
            queryMovieCredits(movieResponse->movieId(), attributes);

        }
        else
        {
            movieResponse->setStatus(tr("Unable to find the selected movie."));
        }
        emit responseReceived(responseId);
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onMovieSearchParsingComplete exception: " << e.what();
    }
}

void MovieViewManager::onMovieDetailsParsingComplete(bool successful)
{
    if (!successful)
    {
        qDebug() << "MovieViewManager::onMovieDetailsParsingComplete: " << "failed to retrieve any movie details";
    }
}

void MovieViewManager::onMovieCreditsParsingComplete( bool successful)
{
    if (!successful)
    {
        qDebug() << "MovieViewManager::onMovieCreditsParsingComplete: " << "failed to retrieve any movie credits";
    }
}

void MovieViewManager::onNetworkQueryTimer()
{
    try
    {
        mNetworkQueryTimer->stop();
        if ( mNetworkQueryDetailsQueue.count() >0)
        {
            auto  networkQueryDetails = mNetworkQueryDetailsQueue.dequeue();
            queryMovieCredits(networkQueryDetails.movieId, networkQueryDetails.creditsAttributes);
            queryMovieDetails (networkQueryDetails.movieId, networkQueryDetails.detailsAttributes );
            mNetworkQueryTimer->start(gQueryTimerIntervalMs);
            mNetworkQueryTimer->start(gQueryTimerIntervalMs);
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onNetworkQueryTimer exception: " << e.what();
    }
}


void MovieViewManager::onUpcomingMoviesParsingComplete( int responseId, bool successful)
{
    try
    {
        auto movieResponse = mUpcomingMoviesResponses.at(responseId);
        if (successful)
        {
            mNetworkQueryTimer->stop();
            movieResponse->setStatus ("");
            mSortedUpcomingMoviesResponses.sortlByTitle();
            QStringList creditsAttributes;
            mMovieAttributes.assignUpcomingMoviesCreditsAttributes(responseId, movieResponse->movieId(), creditsAttributes);
            QStringList detailsAttributes;
            mMovieAttributes.assignUpcomingMoviesDetailsAttributes(responseId, movieResponse->movieId(), detailsAttributes);
            mNetworkQueryDetailsQueue.enqueue(NetworkQueryDetails {movieResponse->movieId(),creditsAttributes, detailsAttributes});
            mNetworkQueryTimer->start(gQueryTimerIntervalMs);
        }
        else
        {
            mUpcomingMoviesResponses.at(responseId)->setStatus(tr("Unable to find any 'Upcoming Movies'"));
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onUpcomingMoviesParsingComplete exception: " << e.what();
    }
}

void MovieViewManager::onNowPlayingParsingComplete(int responseId, bool successful)
{
    try
    {
        auto movieResponse = mNowPlayingMoviesResponses.at(responseId);
        if (successful)
        {
            mNetworkQueryTimer->stop();
            movieResponse->setStatus ("");
            mSortedNowPlayingMoviesResponses.sortlByTitle();
            QStringList creditsAttributes;
            mMovieAttributes.assignMoviesNowPlayingCreditsAttributes(responseId, movieResponse->movieId(), creditsAttributes);
            QStringList detailsAttributes;
            mMovieAttributes.assignMoviesNowPlayingDetailsAttributes(responseId, movieResponse->movieId(), detailsAttributes);
            mNetworkQueryDetailsQueue.enqueue(NetworkQueryDetails {movieResponse->movieId(),creditsAttributes, detailsAttributes});
            mNetworkQueryTimer->start(gQueryTimerIntervalMs);
        }
        else
        {
            mNowPlayingMoviesResponses.at(responseId)->setStatus(tr("Unable to find any 'Movies Now Playing'"));
        }
    }
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onNowPlayingParsingComplete exception: " << e.what();
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
    try
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
    catch(std::exception const & e)
    {
        qDebug() << "MovieViewManager::onShareResponsesFormatted exception: " << e.what();
    }
}




























