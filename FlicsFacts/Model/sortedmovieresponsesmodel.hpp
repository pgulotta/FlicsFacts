#pragma once
#include "../FlicsFacts/fam/qqmlobjectlistmodel.hpp"
#include "../FlicsFacts/Model/movieresponse.hpp"
#include <QSortFilterProxyModel>

class SortedMovieResponsesModel final : public QSortFilterProxyModel
{
Q_OBJECT

public:
    SortedMovieResponsesModel(QQmlObjectListModel<MovieResponse>* movieResponses, QObject *parent )   : QSortFilterProxyModel(parent) {
        // qDebug() << " SortedMovieResponsesModel() called";
        setDynamicSortFilter(false);
        setSourceModel(movieResponses);
    }

//    virtual ~SortedMovieResponsesModel() {
//        qDebug() << "~SortedMovieResponsesModel() called";
//    }

    void sortlByTitle()
    {
        sort(0, Qt::AscendingOrder);
    }

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        MovieResponse * leftMovieResponse = qobject_cast< MovieResponse*>(qvariant_cast<QObject *>( left.data(Qt::UserRole)));
        MovieResponse * rightMovieResponse = qobject_cast< MovieResponse*>(qvariant_cast<QObject *>( right.data(Qt::UserRole)));

        return leftMovieResponse->title() < rightMovieResponse->title();
    }

};
