#pragma once
#include <QObject>


class QByteArray;
class MovieViewManager;


class OmdbResponseParser : public QObject
{
Q_OBJECT

public slots:

signals:
    void searchParsingComplete(int responseId, bool isSuccessful);
    void detailsParsingComplete(int responseId, bool isSuccessful);
    void creditsParsingComplete(int responseId, bool isSuccessful);

public:
    OmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager);
    explicit OmdbResponseParser(const OmdbResponseParser& rhs) = delete;
    OmdbResponseParser& operator= (const OmdbResponseParser& rhs) = delete;
    void parseSearchResult( const QByteArray& mResponseSource, int responseId);
    void parseMovieDetails( const QByteArray& mResponseSource, int responseId);
    void parseMovieCredits( const QByteArray& mResponseSource, int responseId);

private:
    MovieViewManager& mMovieViewManager;

};

