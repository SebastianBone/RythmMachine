// class for left and right screen-border, which start or pause the audiofiles

#include "modqlabel.h"
#include <QDebug>

modQLabel::modQLabel(QWidget *parent) :
    QLabel(parent)
{
    eventEnter = false;
}

// return boolean status
bool modQLabel::isEntered()
{
    return eventEnter;
}

// enter label-area
void modQLabel::enterEvent(QEvent *e)
{
    QLabel::enterEvent(e);
    eventEnter = true;

}

// leave label-area
void modQLabel::leaveEvent(QEvent *e)
{
    QLabel::leaveEvent(e);
    eventEnter = false;

}

