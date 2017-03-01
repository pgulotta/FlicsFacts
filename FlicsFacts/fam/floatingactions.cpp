#include "floatingactions.hpp"


FloatingActions::FloatingActions(QObject *parent) :
    QObject{parent},
    mFloatingActionMenu{new FloatingActionMenu {
        parent
    }},
    mFloatingActionButtons {parent} {
}

FloatingActions::FloatingActions( const QString& menuProperyName, const QString& buttonsProperyName, QQmlContext *context,QObject *parent ) :
    QObject{parent},
    mFloatingActionMenu{new FloatingActionMenu {
        parent
    }},
    mFloatingActionButtons {parent},mMenuProperyName {menuProperyName},mButtonsProperyName {buttonsProperyName}
{
    initializeContext(context);
}

void FloatingActions::initializeContext( QQmlContext *context)
{
    context->setContextProperty(mMenuProperyName, mFloatingActionMenu);
    context->setContextProperty(mButtonsProperyName, &mFloatingActionButtons);
    context->setContextProperty("floatingActionMenu", mFloatingActionMenu);
}
