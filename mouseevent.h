#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>

class mouseevent : public QLabel
{
    Q_OBJECT
public:
    explicit mouseevent(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void leaveEvent(QEvent *);

    int x,y;
    bool isPressed();

signals:

    void mouse_pressed();
    void mouse_pos();
    void mouse_left();
    void mouse_released();

public slots:

};

#endif // MOUSEEVENT_H
