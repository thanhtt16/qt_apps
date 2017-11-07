#ifndef P_SETTINGS_H
#define P_SETTINGS_H

#include <PacModel/presentation.h>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QObject>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <functional>
#include "Common/resources.h"
#include "c_settings.h"

class C_Settings;
class P_Settings : public Presentation {
  Q_OBJECT
 private:
  QWidget *zone;
  QVBoxLayout *zoneLayout;
  QPushButton *closeButton;
  QPushButton *minimizeButton;
  QPushButton *maximizeButton;
  QWidget *titleWidget;
  QLabel *titleLabel;
  QWidget *topWidget;
  QVBoxLayout *topLayout;

  QWidget *toolBarWidget;
  QHBoxLayout *toolBarLayout;

  QStackedWidget *stackWidget;

  QList<QWidget *> zoneList;
  QList<QToolButton *> buttonList;
  int selectedTabIndex = -1;

 protected:
  QPoint oldPos;
  bool isMoving = false;
  bool eventFilter(QObject *watched, QEvent *event);

 public:
  P_Settings(Control *ctrl, QWidget *zone);
  C_Settings *control() { return (C_Settings *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  QWidget *getZone(int zoneId);

  void selectTab(int index);
  void showSettings(int index = 0);
  void hideSettings();

  void initTopZone();
  void initTitleZone();
  void initToolBarZone();
  void initCenterZone();

  void addZone(QString icon, QString title);

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

 public Q_SLOTS:
  void onCloseButtonClicked();
  void onMinimizeButtonClicked();
  void onMaximizeButtonClicked();

  void onSelectedTab();
};

#endif  // P_SETTINGS_H
