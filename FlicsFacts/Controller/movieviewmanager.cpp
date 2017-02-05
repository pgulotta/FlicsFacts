#include "movieviewmanager.hpp"
#include "../FlicsFacts/Controller/shareclient.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QApplication>
#include <QString>
#include <QtNetwork>
#include <QDebug>


const QString gRequest { "http://www.omdbapi.com/?s=%1&y=&plot=full&tomatoes=true&r=json"};

MovieViewManager::MovieViewManager(QObject *parent) :
    QObject{parent},
    m_requestFailed{tr("This request was unsuccessful.")},
    m_appName{QApplication::applicationName()},
    m_appVersion{"1.06"},
    mShareResponsesFormatterformatter{parent},
    mShareResponsesWatcher{parent},
    mOmdbResponseParser{parent, *this}
{
    connect(&mNetworkAccessManager, &QNetworkAccessManager::finished, this, &MovieViewManager::onNetworkReply);
    connect(&mShareResponsesWatcher, &QFutureWatcher<QString>::finished, this, &MovieViewManager::onShareResponsesFormatted);
    connect(&mOmdbResponseParser, &OmdbResponseParser::parsingComplete, this,  &MovieViewManager::onParsingComplete);
}

void MovieViewManager::findFlicSelected(QString movieTitle)
{
    qDebug() << "MovieViewManager::findFlicSelected: movieTitle="<< movieTitle;
    if (movieTitle.isEmpty())
        return;

    int responseIndex = static_cast<int>(mMovieResponses.size());
    mMovieResponses.emplace_back( std::make_unique<MovieResponse>() );
    emit requestCreated(movieTitle, responseIndex);

    queryMovieDetails(responseIndex, movieTitle);
}

QString MovieViewManager::formatUrl(QString movieTitle)
{
    return (QString ( gRequest.arg(movieTitle.trimmed().replace(' ','+') )  ) );;
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

void MovieViewManager::queryMovieDetails(int responseId, QString movieTitle)
{
    auto request = QNetworkRequest( formatUrl(movieTitle));
    request.setAttribute(QNetworkRequest::Attribute::User, QVariant(responseId ));
    mNetworkAccessManager.get(request);
    qDebug() << "MovieViewManager::queryMovieDetails: id="<< responseId << "  url=" << request.url();
}

void MovieViewManager::onNetworkReply(QNetworkReply *networkReply)
{
    int responseId = networkReply->request().attribute(QNetworkRequest::Attribute::User).toInt();
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
        if ( source != nullptr && source.size() >0)
        {
            mOmdbResponseParser.parse(source,  responseId);
        }
    }
    networkReply->deleteLater();
}

void MovieViewManager::onParsingComplete(int responseId, bool successful)
{
    if (successful)
    {
        setStatus ( responseId, "");
    }
    else
    {
        setStatus ( responseId, QString (tr("Unable to find the selected flic.")));
    }
    emit responseReceived(responseId);
}

void MovieViewManager::setStatus(int responseId, QString status)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Status == status)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Status = status;
}

void MovieViewManager::setYear(int responseId, QString year)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Year == year)
        return;

    mMovieResponses.at(static_cast<std::size_t>(responseId))->Year = year;
}

void MovieViewManager::setTitle(int responseId,QString title)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Title == title)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Title = title;
}

void MovieViewManager::setPoster(int responseId,QString poster)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Poster == poster)
        return;

    mMovieResponses.at(static_cast<std::size_t>(responseId) )->Poster = poster;
}

void MovieViewManager::setGenre(int responseId, QString genre)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre == genre)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Genre = genre;
}

void MovieViewManager::setImdbRating(int responseId,QString imdbRating)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->ImdbRating == imdbRating)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->ImdbRating = imdbRating;
}

void MovieViewManager::setTomatoRating(int responseId,QString tomatoRating)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->TomatoRating == tomatoRating)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->TomatoRating = tomatoRating;
}

void MovieViewManager::setWebsite(int responseId,QString website)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Website == website)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Website = website;
}

void MovieViewManager::setRated(int responseId,QString rated)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated == rated)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Rated = rated;
}

void MovieViewManager::setReleased(int responseId,QString released)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Released == released)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Released = released;
}

void MovieViewManager::setRuntime(int responseId,QString runtime)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime == runtime)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Runtime = runtime;
}

void MovieViewManager::setActors(int responseId,QString actors)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors == actors)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Actors = actors;
}

void MovieViewManager::setPlot(int responseId,QString plot)
{
    if ( mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot == plot)
        return;
    mMovieResponses.at(static_cast<std::size_t>(responseId))->Plot = plot;
}

void MovieViewManager::setTitleRequest(QString titleRequest)
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
































