//own Qclass for soundbutton behavior


#include "modqpushbutton.h"

ModQPushButton::ModQPushButton(QWidget *parent) :
    QPushButton(parent)
{
    // timer that delays the checking when hovering over a button

    delayTimer = new QTimer(this);
    delayTimer->setInterval(1000);
    delayTimer->setTimerType(Qt::PreciseTimer);
    connect(delayTimer, SIGNAL(timeout()), this, SLOT(delay()));

}

// start timer when entering buttonarea

void ModQPushButton::enterEvent(QEvent *e)
{
    QPushButton::enterEvent(e);
    delayTimer->start();

}

// stop timer when leaving buttonarea

void ModQPushButton::leaveEvent(QEvent *e)
{
    QPushButton::leaveEvent(e);
    delayTimer->stop();

}

// change status of button

void ModQPushButton::delay()
{
    if (QPushButton::isChecked()== false)
        QPushButton::setChecked(true);
    else
        QPushButton::setChecked(false);

    delayTimer->stop();

}
