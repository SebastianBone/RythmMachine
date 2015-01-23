#include "modqpushbutton.h"

ModQPushButton::ModQPushButton(QWidget *parent) :
    QPushButton(parent)
{
    delayTimer = new QTimer(this);
    delayTimer->setInterval(1000);
    delayTimer->setTimerType(Qt::PreciseTimer);
    connect(delayTimer, SIGNAL(timeout()), this, SLOT(delay()));

}

void ModQPushButton::enterEvent(QEvent *e)
{
    QPushButton::enterEvent(e);
    delayTimer->start();

    //test
    qDebug()<< "ENTERED";

}

void ModQPushButton::leaveEvent(QEvent *e)
{
    QPushButton::leaveEvent(e);
    delayTimer->stop();

    //test
    qDebug()<< "LEAVED";

}

void ModQPushButton::delay()
{
    if (QPushButton::isChecked()== false)
        QPushButton::setChecked(true);
    else
        QPushButton::setChecked(false);

    delayTimer->stop();

}
