#ifndef P_PBDATAMAPTAB_H
#define P_PBDATAMAPTAB_H

#include <PacModel/presentation.h>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "c_pb_datamaptab.h"
class C_PBDataMapTab;
class P_PBDataMapTab : public Presentation {
  // init ui control
 private:
  QWidget *calendarZone;
  QWidget *playlistZone;

 public:
  P_PBDataMapTab(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update();
  void show(QVariant *attactment);
  QWidget *getZone(int zoneId);
};

#endif  // P_PBDATAMAPTAB_H
