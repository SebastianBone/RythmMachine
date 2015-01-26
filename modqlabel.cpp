#include "modqlabel.h"
#include <QDebug>

modQLabel::modQLabel(QWidget *parent) :
    QLabel(parent)
{
    eventEnter = false;
}

bool modQLabel::isEntered()
{
    return eventEnter;
}

void modQLabel::enterEvent(QEvent *e)
{
    QLabel::enterEvent(e);
    eventEnter = true;

}

void modQLabel::leaveEvent(QEvent *e)
{
    QLabel::leaveEvent(e);
    eventEnter = false;

}

