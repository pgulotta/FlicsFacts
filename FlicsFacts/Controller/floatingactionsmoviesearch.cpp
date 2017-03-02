#include "floatingactionsmoviesearch.hpp"


FloatingActionsMovieSearch::FloatingActionsMovieSearch(const QString& menuProperyName, const QString& buttonsProperyName, QQmlContext *context, QObject *parent) :
    FloatingActions{menuProperyName,buttonsProperyName,context, parent}{
}

void FloatingActionsMovieSearch::onButtonSelected(const QString& contextId,int modelIndex)
{
    if( contextId == mButtonsProperyName)
        qDebug() << "FloatingActionsMovieSearch buttonSelected:  modelIndex=" << modelIndex << "contextId: " << contextId;
}

void FloatingActionsMovieSearch::onMenuSelected(const QString& contextId,bool isClosed)
{
    if( contextId == mMenuProperyName)
        qDebug() << "FloatingActionsMovieSearch menuSelected:  isClosed=" << isClosed << "contextId: " << contextId <<"  mButtonsProperyName:" <<mButtonsProperyName;
}


void FloatingActionsMovieSearch::initialize(QObject *parent)
{
    mFloatingActionMenu->setLabelBackColor("LAVENDER");
    mFloatingActionMenu->setIconColor("slategrey");

    mFloatingActionButtons.append( new FloatingActionButton {parent, "","qrc:/image/edit.png", "blue"});
    mFloatingActionButtons.append( new FloatingActionButton {parent, tr("Add New"),"qrc:/image/add.png", "green"});
    mFloatingActionButtons.append( new FloatingActionButton {parent, tr("Delete Current"),"qrc:/image/delete.png", "red"});
}

void FloatingActionsMovieSearch::assignSlots(QObject* rootObject)
{
    QObject::connect (rootObject, SIGNAL(menuSelected(QString,bool)),this, SLOT(onMenuSelected(QString,bool)));
    QObject::connect (rootObject, SIGNAL(buttonSelected(QString,int)),this, SLOT(onButtonSelected(QString,int)));
}
