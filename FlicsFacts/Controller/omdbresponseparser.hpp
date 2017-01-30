#pragma once
#include <QObject>


class QByteArray;
class MovieViewManager;


class OmdbResponseParser : public QObject
{
Q_OBJECT

public slots:

signals:
    void parsingComplete(int responseId, bool isSuccessful);

public:
    OmdbResponseParser(QObject *parent, MovieViewManager& movieViewManager);
    explicit OmdbResponseParser(const OmdbResponseParser& rhs) = delete;
    OmdbResponseParser& operator= (const OmdbResponseParser& rhs) = delete;
    void parse( const QByteArray& mResponseSource, int responseId);

private:
    MovieViewManager& mMovieViewManager;
};

