#ifndef P_PBLAYOUTPAGE_H
#define P_PBLAYOUTPAGE_H

#include <PacModel/presentation.h>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "c_pb_layoutpage.h"

class C_PBLayoutPage;

class P_PBLayoutPage : public Presentation {
 private:
  QWidget *pageZone;
  QWidget *layoutZone;
  QWidget *playListZone;
  QWidget *calendarZone;
  QPushButton *showSettingButton = Q_NULLPTR;

 public:
  P_PBLayoutPage(Control *ctrl, QWidget *zone);
  C_PBLayoutPage *control() { return (C_PBLayoutPage *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  void show(QVariant *attactment);
  QWidget *getZone(int zoneId);
};

#endif  // P_PBLAYOUTPAGE_H
