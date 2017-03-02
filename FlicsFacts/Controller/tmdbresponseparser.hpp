#pragma once
#include <QObject>


class QByteArray;
class MovieViewManager;


class TmdbResponseParser : public QObject
{
Q_OBJECT

public slots:

signals:
    void searchParsingComplete(int responseId, bool isSuccessful);
    void detailsParsingComplete(int responseId, bool isSuccessful);
    void creditsParsingComplete(int responseId, bool isSuccessful);

public:
    TmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager);
    explicit TmdbResponseParser(const TmdbResponseParser& rhs) = delete;
    TmdbResponseParser& operator= (const TmdbResponseParser& rhs) = delete;
    void parseSearchResult( const QByteArray& mResponseSource, int responseId);
    void parseMovieDetails( const QByteArray& mResponseSource, int responseId);
    void parseMovieCredits( const QByteArray& mResponseSource, int responseId);

private:
    MovieViewManager& mMovieViewManager;

};

