#ifndef P_About_H
#define P_About_H

#include <PacModel/presentation.h>
#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QMovie>
#include <QThread>
#include <QTimer>
#include <QPushButton>
#include <QSettings>
#include <QDesktopWidget>
#include "MainFrame/AboutApp/c_about.h"
#include <Common/resources.h>
class C_About;
class P_About;
class P_About : public Presentation {
private:

public:
    //init title
    QVBoxLayout *zoneLayout;
    QPushButton *closeButton;
    QPushButton *minimizeButton;
    QPushButton *maximizeButton;
    QWidget *titleWidget;
    QLabel *titleLabel;
    QWidget *topWidget;
    QVBoxLayout *topLayout;

    QWidget *widgetMain = Q_NULLPTR;
    QWidget *zone  = Q_NULLPTR;
    P_About(Control *ctrl, QWidget *zone);
    C_About *control() { return (C_About *)this->ctrl; }



    void update();
    void showAboutApp();
    void initTopZone();
    void initTitleZone();
    QPushButton *createButton(QWidget *parent, QSize size, QRect borderRadius,
                              int borderWidth, QString backngroundColor,
                              QString textColor, QString borderColor,
                              QString text) {
        QPushButton *button = new QPushButton(parent);
        button->setFixedSize(size);
        QString css =
                "background-color: %1; color: %2; border: %3px solid %4;"
                "border-top-left-radius: %5; border-top-right-radius: %6; "
                "border-bottom-left-radius: %7; border-bottom-right-radius: %8; ";
        button->setStyleSheet(css.arg(backngroundColor)
                              .arg(textColor)
                              .arg(borderWidth)
                              .arg(borderColor)
                              .arg(borderRadius.x())
                              .arg(borderRadius.y())
                              .arg(borderRadius.width())
                              .arg(borderRadius.height()));
        button->setText(text);
        button->setFont(Resources::instance().getExtraSmallRegularButtonFont());
        return button;
    }
    QObject *getZone(int zoneId);
//protected:
//    QPoint oldPos;
//    bool isMoving = false;
//    bool eventFilter(QObject *watched, QEvent *event);

public Q_SLOTS:
    void onCloseButtonClicked();
    void onMinimizeButtonClicked();
    void onMaximizeButtonClicked();


};

#endif  // PRESENTATION_H
