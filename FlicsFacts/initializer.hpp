#pragma once

#include <Controller/movieviewmanager.hpp>
#include <QQmlApplicationEngine>
#include <QObject>

class Initializer : public QObject
{
public:
    Q_OBJECT

public:
    explicit Initializer(const Initializer& ) = delete;
    Initializer& operator= (const Initializer&) = delete;

    explicit Initializer(QObject *parent=0);

private:
    QQmlApplicationEngine mQmlApplicationEngine;
    MovieViewManager mMovieViewManager;


};

