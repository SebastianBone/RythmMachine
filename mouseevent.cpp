#include "mouseevent.h"


mouseevent::mouseevent(QWidget *parent) :
    QLabel(parent)
{
}

bool mousePressed = false;

void mouseevent::mouseMoveEvent(QMouseEvent *ev)
{
    this->x = ev->x();
    this->y = ev->y();
    emit mouse_pos();
}

void mouseevent::mousePressEvent(QMouseEvent *ev)
{

    //if(ev->buttons() == Qt::LeftButton){
    mousePressed = true;
    qDebug() << "PRESSED" << mousePressed;
    emit mouse_pressed();

}

void mouseevent::leaveEvent(QEvent *)
{
    emit mouse_left();
}

bool mouseevent:: isPressed(){
    return mousePressed;
}


void mouseevent::mouseReleaseEvent(QMouseEvent *ev)
{

    mousePressed = false;
    qDebug() << "RELEASED" << !mousePressed;

    emit mouse_released();
}
