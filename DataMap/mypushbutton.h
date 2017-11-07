#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include<QPushButton>
#include<QPainter>

class MyPushButton : public QPushButton
{
    Q_OBJECT

public:
    MyPushButton(QWidget *parent = 0)
        : QPushButton(parent)
    { }

    void paintEvent(QPaintEvent *p)
    {
        QPushButton::paintEvent(p);
        QPainter paint(this);
        paint.drawText(QPoint(10,10),"");
    }
};
#endif // MYPUSHBUTTON_H
