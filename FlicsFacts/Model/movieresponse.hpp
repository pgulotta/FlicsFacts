#pragma once

#include <QtQml>
#include <QGuiApplication>
#include <QScreen>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QDate>
#include <QSortFilterProxyModel>


class MovieResponse : public QObject
{
Q_OBJECT
Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged )
Q_PROPERTY(QString year READ year WRITE setYear NOTIFY yearChanged)
Q_PROPERTY(QString rating READ rating WRITE setRating NOTIFY ratingChanged)
Q_PROPERTY(QString released READ released WRITE setReleased NOTIFY releasedChanged)
Q_PROPERTY(QString runtime READ runtime WRITE setRuntime NOTIFY runtimeChanged)
Q_PROPERTY(QString actors READ actors WRITE setActors NOTIFY actorsChanged)
Q_PROPERTY(QString plot READ plot WRITE setPlot NOTIFY plotChanged)
Q_PROPERTY(QString popularity READ popularity WRITE setPopularity NOTIFY popularityChanged)
Q_PROPERTY(QString languages READ languages WRITE setLanguages NOTIFY languagesChanged)
Q_PROPERTY(QString website READ website WRITE setWebsite NOTIFY websiteChanged)
Q_PROPERTY(QString websiteUrl READ websiteUrl WRITE setWebsiteUrl NOTIFY websiteUrlChanged)
Q_PROPERTY(QString poster READ poster WRITE setPoster NOTIFY posterChanged)
Q_PROPERTY(QString genre READ genre WRITE setGenre NOTIFY genreChanged)
Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
Q_PROPERTY(int movieId READ movieId WRITE setMovieId NOTIFY movieIdChanged)

public slots:

    void setMovieId(int movieId)
    {
        if (m_movieId == movieId)
            return;

        m_movieId = movieId;
        emit movieIdChanged(movieId);
    }

    void setTitle(QString title)
    {
        if (m_title == title)
            return;
        m_title= (title.length() >mMaxValueLength ) ? title.left(mTruncateLength) + elideText : title;
        emit titleChanged(title);
    }

    void setYear(QString year)
    {
        if (m_year == year)
            return;

        m_year = year;
        emit yearChanged(year);
    }

    void setRating(QString rating)
    {
        if (m_rating == rating)
            return;

        m_rating = rating;
        emit ratingChanged(rating);
    }

    void setReleased(QString released)
    {
        if (m_released == released)
            return;

        m_released = released;
        emit releasedChanged(released);
    }

    void setRuntime(QString runtime)
    {
        if (m_runtime == runtime)
            return;

        m_runtime = runtime;
        emit runtimeChanged(runtime);
    }

    void setActors(QString actors)
    {
        if (m_actors == actors)
            return;

        m_actors = actors;
        emit actorsChanged(actors);
    }

    void setPlot(QString plot)
    {
        if (m_plot == plot)
            return;

        m_plot = plot;
        emit plotChanged(plot);
    }

    void setPopularity(QString popularity)
    {
        if (m_popularity == popularity)
            return;

        m_popularity = popularity;
        emit popularityChanged(popularity);
    }

    void setLanguages(QString languages)
    {
        if (m_languages == languages)
            return;

        m_languages = languages;
        emit languagesChanged(languages);
    }

    void setWebsite(QString website)
    {
        if (m_website == website)
            return;

        m_website = website;
        emit websiteChanged(website);
    }

    void setWebsiteUrl(QString websiteUrl)
    {
        if (m_websiteUrl == websiteUrl)
            return;

        m_websiteUrl = websiteUrl;
        emit websiteUrlChanged(websiteUrl);
    }

    void setPoster(QString poster)
    {
        if (m_poster == poster)
            return;

        m_poster = poster;
        emit posterChanged(poster);
    }

    void setStatus(QString status)
    {
        if (m_status == status)
            return;

        m_status = status;
        emit statusChanged(status);
    }

    void setGenre(QString genre)
    {
        if (m_genre == genre)
            return;

        m_genre = genre;
        emit genreChanged(genre);
    }

signals:
    void movieIdChanged(int movieId);

    void titleChanged(QString title);

    void yearChanged(QString year);

    void ratingChanged(QString rating);

    void releasedChanged(QString released);

    void runtimeChanged(QString runtime);

    void actorsChanged(QString actors);

    void plotChanged(QString plot);

    void popularityChanged(QString popularity);

    void languagesChanged(QString languages);

    void websiteChanged(QString website);

    void websiteUrlChanged(QString websiteUrl);

    void posterChanged(QString poster);

    void statusChanged(QString status);

    void genreChanged(QString genre);

public:
    explicit MovieResponse(QObject *parent = nullptr) :
        QObject(parent){
        //  qDebug() << " MovieResponse() called";
    }

    MovieResponse(const QString& title,  QObject *parent = nullptr) :
        QObject(parent){
        // qDebug() << " MovieResponse() called";
        m_title= title;
    }

//    virtual ~MovieResponse()
//    {
//        qDebug() << " ~MovieResponse() called";
//    }

    int movieId() const
    {
        return m_movieId;
    }

    QString title() const
    {
        return m_title;
    }

    QString year() const
    {
        return m_year;
    }

    QString rating() const
    {
        return m_rating;
    }

    QString released() const
    {
        return m_released;
    }

    QString runtime() const
    {
        return m_runtime;
    }

    QString actors() const
    {
        return m_actors;
    }

    QString plot() const
    {
        return m_plot;
    }

    QString popularity() const
    {
        return m_popularity;
    }

    QString languages() const
    {
        return m_languages;
    }

    QString website() const
    {
        return m_website;
    }

    QString websiteUrl() const
    {
        return m_websiteUrl;
    }

    QString poster() const
    {
        return m_poster;
    }

    QString status() const
    {
        return m_status;
    }

    QString genre() const
    {
        return m_genre;
    }

private:
    QString m_title;
    QString m_year;
    QString m_rating;
    QString m_released;
    QString m_runtime;
    QString m_actors;
    QString m_plot;
    QString m_popularity;
    QString m_languages;
    QString m_website;
    QString m_websiteUrl;
    QString m_poster;
    QString m_genre;
    QString m_status ={tr("Working ...")};
    int m_movieId;

    const QString elideText {"..."};
    const int mMaxValueLength {25};
    const int mTruncateLength {24};

};


