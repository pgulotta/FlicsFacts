#pragma once

#include "fam/floatingactions.hpp"
#include <QDebug>


class FloatingActionsMovieSearch : public FloatingActions
{

public slots:
    void onButtonSelected(const QString&  contextId,int modelIndex) override final;
    void onMenuSelected(const QString&  contextId,bool isClosed) override final;

public:
    FloatingActionsMovieSearch( const QString& menuProperyName, const QString& buttonsProperyName,QQmlContext *context, QObject *parent = nullptr);
    void initialize(QObject *parent)  override final;
    void assignSlots(QObject* rootObject);
//    ~FloatingActionsMovieSearch()
//    {
//        qDebug() << " ~FloatingActionsMovieSearch() called";
//    }

private:


};

