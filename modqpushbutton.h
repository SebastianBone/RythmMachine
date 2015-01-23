#ifndef MODQPUSHBUTTON_H
#define MODQPUSHBUTTON_H

#include <QPushButton>
#include <QEvent>
#include <QDebug>
#include <QObject>
#include <QTimer>

class ModQPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ModQPushButton(QWidget *parent = 0);

private:
    QTimer *delayTimer;

protected:
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);


signals:

public slots:
    void delay();



};

#endif // MODQPUSHBUTTON_H
