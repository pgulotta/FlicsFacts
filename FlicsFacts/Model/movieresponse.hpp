#pragma once
#include <QObject>
#include <QString>

struct  MovieResponse
{
    MovieResponse(){
        Status =  QObject::tr("Working ...");
    }
    int MovieId;
    QString Title;
    QString Year;
    QString Rated;
    QString Released;
    QString Runtime;
    QString Actors;
    QString Plot;
    QString ImdbRating;
    QString TomatoRating;
    QString Website;
    QString Poster;
    QString Genre;
    QString Status;
};



