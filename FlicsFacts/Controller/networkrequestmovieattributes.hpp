#pragma once

#include <QStringList>
#include <QString>
#include <QDebug>

enum class queryType
{
    undefined,
    movieSearch,
    movieSearchDetails,
    movieSearchCredits,
    moviesNowPlaying,
    moviesNowPlayingDetails,
    moviesNowPlayingCredits,
    upcomingMovies,
    upcomingMoviesDetails,
    upcomingMoviesCredits
};


class NetworkRequestMovieAttributes
{
public:
    const int NoResourceId = -1;

    void assignMovieSearchAttributes(int responseId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::movieSearch) ));
        attributes.append( QString::number( responseId));
    }

    void assignMovieSearchCreditsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::movieSearchCredits) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }

    void assignMovieSearchDetailsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::movieSearchDetails) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }

    void assignMoviesNowPlayingAttributes(int responseId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::moviesNowPlaying) ));
        attributes.append( QString::number( responseId));
    }

    void assignMoviesNowPlayingCreditsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::moviesNowPlayingCredits) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }

    void assignMoviesNowPlayingDetailsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::moviesNowPlayingDetails) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }

    void assignUpcomingMoviesAttributes(int responseId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::upcomingMovies) ));
        attributes.append( QString::number( responseId));
    }

    void assignUpcomingMoviesCreditsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::upcomingMoviesCredits) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }

    void assignUpcomingMoviesDetailsAttributes(int responseId, int movieId, QStringList& attributes) const
    {
        attributes.clear();
        attributes.append( QString::number( static_cast<int>(queryType::upcomingMoviesDetails) ));
        attributes.append( QString::number( responseId));
        attributes.append( QString::number( movieId));
    }
    queryType  extractQueryTypeAttributes(const QStringList& attributes) const
    {
        queryType result = queryType::undefined;
        try {
            result   = static_cast<queryType>(attributes[0].toInt());
        }
        catch(std::exception const & e)
        {
            qDebug() << "NetworkRequestMovieAttributes::extractQueryTypeAttributes exception: " << e.what();
        }

        return result;
    }

    int extractResourceIdAttributes(const QStringList& attributes) const
    {
        int result = NoResourceId;
        try {
            result   = static_cast<int>(attributes[1].toInt());
        }
        catch(std::exception const & e)
        {
            qDebug() << "NetworkRequestMovieAttributes::extractResourceIdAttributes exception: " << e.what();
        }

        return result;
    }

    int extractMovieIdAttributes(const QStringList& attributes) const
    {
        int result = NoResourceId;
        try {
            result   = static_cast<int>(attributes[2].toInt());
        }
        catch(std::exception const & e)
        {
            qDebug() << "NetworkRequestMovieAttributes::extractMovieIdAttributes exception: " << e.what();
        }

        return result;
    }

private:


};



