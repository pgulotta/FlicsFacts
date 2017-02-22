#include "movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareclient.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QApplication>
#include <QString>
#include <QtNetwork>
#include <QDebug>

const QString gSearchRequest { "http://api.themoviedb.org/3/search/movie?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US&query=%1&page=1&include_adult=false"};
const QString gDetailsRequest {"http://api.themoviedb.org/3/movie/%1?api_key=2839bfb130659459d7d9972ad9aa3cd4&language=en-US"};


MovieViewManager::MovieViewManager(QObject *parent) :
    QObject{parent},
    m_requestFailed{tr("This request was unsuccessful.")},
    m_appName{QApplication::applicationName()},
    m_appVersion{"1.07"},
    mShareResponsesFormatterformatter{parent},
    mShareResponsesWatcher{parent},
    mOmdbResponseParser{parent, *this}
{
    connect(&mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MovieViewManager::onNetworkReply);
    connect(&mShareResponsesWatcher, &QFutureWatcher<QString>::finished, this, &MovieViewManager::onShareResponsesFormatted);
    connect(&mOmdbResponseParser, &OmdbResponseParser::searchParsingComplete, this,  &MovieViewManager::onSearchParsingComplete);
    connect(&mOmdbResponseParser, &OmdbResponseParser::detailsParsingComplete, this,  &MovieViewManager::onDetailsParsingComplete);
}

void MovieViewManager::setMovieId(int responseId, int movieId)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId == movieId)
        return;

    mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId = movieId;
}

void MovieViewManager::findFlicSelected(const QString& movieTitle)
{
    qDebug() << "MovieViewManager::findFlicSelected: movieTitle="<< movieTitle;
    if (movieTitle.isEmpty())
        return;

    int responseIndex = static_cast<int>(mMovieResponses.size());
    mMovieResponses.emplace_back( std::make_unique<MovieResponse>() );
    emit requestCreated(movieTitle, responseIndex);

    queryMovieSearch(responseIndex, movieTitle);
}

QString MovieViewManager::formatMovieSearchUrl(const QString& movieTitle)
{
    return (QString ( gSearchRequest.arg(movieTitle.trimmed().replace(' ','+') )  ) );;
}

QString MovieViewManager::formatMovieDetailsUrl(int movieId)
{
    return (QString ( gDetailsRequest.arg(movieId) ) );;
}

int MovieViewManager::removeSelectedMovie(int responseId)
{
    mMovieResponses.erase(mMovieResponses.begin() + responseId);
    return static_cast<int>( mMovieResponses.size());
}

void MovieViewManager::shareMovieResponses()
{
    QFuture<QString> future = QtConcurrent::run<QString>(&mShareResponsesFormatterformatter, &ShareResponsesFormatter::formatAsText,
                                                         mMovieResponses.cbegin(), mMovieResponses.cend());
    mShareResponsesWatcher.setFuture(future);
}

void MovieViewManager::onShareResponsesFormatted()
{
    if ( mShareResponsesWatcher.result().size() == 0)
    {
        emit displayTextMessage( tr( "Share FlicsFacts"), tr("There is nothing to share."));
    }
    else
    {
        ShareClient shareClient(parent());
        shareClient.setShare(mShareResponsesWatcher.result());
    }
}

void MovieViewManager::queryMovieDetails(int responseId, int movieId)
{
    auto request = QNetworkRequest( formatMovieDetailsUrl(movieId));
    QStringList attributes {QString::number(responseId), QString::number(movieId)};
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
    mNetworkAccessManager.get(request);
    qDebug() << "MovieViewManager::queryMovieDetails: id="<< responseId << "  url=" << request.url();
}

void MovieViewManager::queryMovieSearch(int responseId,const QString& movieTitle)
{
    auto request = QNetworkRequest( formatMovieSearchUrl(movieTitle));
    QStringList attributes {QString::number( responseId)};
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(attributes ));
    mNetworkAccessManager.get(request);
    qDebug() << "MovieViewManager::queryMovieSearch: id="<< responseId << "  url=" << request.url();
}
void MovieViewManager::onNetworkReply(QNetworkReply *networkReply)
{
    QStringList attributes = networkReply->request().attribute(QNetworkRequest::Attribute::User).toStringList();
    int responseId = attributes.at(0).toInt();
    if ( networkReply->error())
    {
        auto errorMessage = networkReply->errorString().length() > 50    ? "" : networkReply->errorString();
        setStatus ( responseId, QString ("%1\n%2").arg(m_requestFailed).arg( errorMessage));
        emit responseReceived(responseId);
        qDebug() << "MovieViewManager::onNetworkReply: failed: " << networkReply->errorString();
    }
    else
    {
        const QByteArray source = networkReply->readAll();
        if ( attributes.count()>1)
        {
            mOmdbResponseParser.parseMovieDetails(source, responseId);
        }
        else
        {
            mOmdbResponseParser.parseSearchResult(source, responseId);
        }
    }
    networkReply->deleteLater();
}

void MovieViewManager::onSearchParsingComplete(int responseId, bool successful)
{
    qDebug() << "MovieViewManager::onSearchParsingComplete for responseId=" << responseId << " successful=" << successful;
    if (successful)
    {
        setStatus ( responseId, "");
        queryMovieDetails (responseId, mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId);
    }
    else
    {
        setStatus ( responseId, QString (tr("Unable to find the selected flic.")));
    }
    emit responseReceived(responseId);
}

void MovieViewManager::onDetailsParsingComplete(int responseId, bool successful)
{
    if (successful)
    {

        emit responseReceived(responseId);
    }
    qDebug() << "MovieViewManager::onDetailsParsingComplete for responseId=" << responseId << " successful=" << successful;
}

void MovieViewManager::setStatus(int responseId, const QString& status)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Status == status)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Status = status;
}

void MovieViewManager::setYear(int responseId, const QString&  year)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Year == year)
        return;

    mMovieResponses.at(static_cast<std::size_t>(responseId))->Year = year;
}

void MovieViewManager::setTitle(int responseId,const QString&  title)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Title == title)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Title = title;
}

void MovieViewManager::setPoster(int responseId,const QString&  poster)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Poster == poster)
        return;

    mMovieResponses.at(static_cast<std::size_t>(responseId) )->Poster = poster;
}

void MovieViewManager::setGenre(int responseId, const QString&  genre)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre == genre)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre = genre;
}

void MovieViewManager::setImdbRating(int responseId,const QString&  imdbRating)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->ImdbRating == imdbRating)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->ImdbRating = imdbRating;
}

void MovieViewManager::setTomatoRating(int responseId,const QString&  tomatoRating)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->TomatoRating == tomatoRating)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->TomatoRating = tomatoRating;
}

void MovieViewManager::setWebsite(int responseId,const QString&  website)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Website == website)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Website = website;
}

void MovieViewManager::setRated(int responseId,const QString&  rated)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated == rated)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated = rated;
}

void MovieViewManager::setReleased(int responseId,const QString&  released)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Released == released)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Released = released;
}

void MovieViewManager::setRuntime(int responseId,const QString&  runtime)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime == runtime)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime = runtime;
}

void MovieViewManager::setActors(int responseId,const QString&  actors)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors == actors)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors = actors;
}

void MovieViewManager::setPlot(int responseId,const QString&  plot)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot == plot)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot = plot;
}

void MovieViewManager::setTitleRequest(const QString&  titleRequest)
{
    if (m_titleRequest == titleRequest)
        return;

    m_titleRequest = titleRequest;
    emit titleRequestChanged(titleRequest);
}

QString MovieViewManager::year(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Year;
}

QString MovieViewManager::rated(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated;
}

QString MovieViewManager::released(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Released;
}

QString MovieViewManager::runtime(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime;
}

QString MovieViewManager::actors(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors;
}

QString MovieViewManager::plot(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot;
}

QString MovieViewManager::imdbRating(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->ImdbRating;
}

int MovieViewManager::movieId(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->MovieId;
}

QString MovieViewManager::tomatoRating(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->TomatoRating;
}

QString MovieViewManager::website(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Website;
}

QString MovieViewManager::title(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Title;
}

QString MovieViewManager::poster(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Poster;
}

QString MovieViewManager::genre(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre;
}

QString MovieViewManager::status(int responseId) const
{
    return mMovieResponses.at(static_cast<std::size_t>(responseId))->Status;
}
































