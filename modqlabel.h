#ifndef MODQLABEL_H
#define MODQLABEL_H

#include <QLabel>

class modQLabel : public QLabel
{
    Q_OBJECT
public:
    explicit modQLabel(QWidget *parent = 0);
    bool eventEnter;

signals:

public slots:
    bool isEntered();

protected:
    virtual void enterEvent(QEvent *e);
    virtual void leaveEvent(QEvent *e);


};

#endif // MODQLABEL_H
