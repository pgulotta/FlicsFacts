#pragma once

#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include "../FlicsFacts/Model/movieresponse.hpp"
#include <QObject>


class QByteArray;
class MovieViewManager;


class MovieSearchParser final : public QObject
{
Q_OBJECT

public slots:

signals:
    void movieSearchParsingComplete(int responseId, bool isSuccessful);
    void nowPlayingParsingComplete(int responseId, bool isSuccessful);
    void upcomingMoviesParsingComplete(int responseId, bool isSuccessful);
    void detailsParsingComplete( bool isSuccessful);
    void creditsParsingComplete( bool isSuccessful);

public:
    explicit MovieSearchParser(QObject *parent);
    explicit MovieSearchParser(const MovieSearchParser& rhs) = delete;
    MovieSearchParser& operator= (const MovieSearchParser& rhs) = delete;
    void parseMovieSearchResult(const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination,  int responseId);
    void parseNowPlaying( const QByteArray &source, QQmlObjectListModel<MovieResponse>& destination, int responseId);
    void parseUpcomingMovies(const QByteArray &source, QQmlObjectListModel<MovieResponse>& destination, int responseId);
    void parseMovieDetails(const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination, int responseId);
    void parseMovieCredits(const QByteArray& source, QQmlObjectListModel<MovieResponse>& destination, int responseId);

private:
    void parseMovieResponse(const QJsonObject &jsonObject, MovieResponse* movieResponse) const;


};

