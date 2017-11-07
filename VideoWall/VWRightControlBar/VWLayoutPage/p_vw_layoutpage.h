#ifndef P_VWLAYOUTPAGE_H
#define P_VWLAYOUTPAGE_H

#include <PacModel/presentation.h>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "c_vw_layoutpage.h"

class C_VWLayoutPage;

class P_VWLayoutPage : public Presentation {
 private:
  QWidget *pageZone;
  QWidget *layoutZone;
  QPushButton *showSettingButton = Q_NULLPTR;

 public:
  P_VWLayoutPage(Control *ctrl, QWidget *zone);
  C_VWLayoutPage *control() { return (C_VWLayoutPage *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  void show(QVariant *attactment);
  QWidget *getZone(int zoneId);

 public Q_SLOTS:
};

#endif  // P_VWLAYOUTPAGE_H
