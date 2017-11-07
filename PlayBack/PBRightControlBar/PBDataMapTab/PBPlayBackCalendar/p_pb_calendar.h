#ifndef P_PBCAlENDAR_H
#define P_PBCAlENDAR_H

#include <PacModel/presentation.h>
#include <QCalendarWidget>
#include <QDateTime>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include "Common/generaldefine.h"
#include "Common/resources.h"
#include "c_pb_calendar.h"
#include "DataMap/calendarcustomize.h"
#include <QTextCharFormat>
class C_PBCalendar;
class P_PBCalendar : public Presentation {
  // init ui control
 private:
 public:
  QDateTime currentDate;
  P_PBCalendar(Control *ctrl, QWidget *zone);
  C_PBCalendar *control() { return (C_PBCalendar *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();

  QObject *getZone(int zoneId);

  CalendarCustomize *mCalendarVOD;
  void updateStateOfDate(QList<QHash<QDate, int>> listStateDates);
  void selectDefaultDate();
  QList<QDate> caculatorDates(QDate minDate,QDate maxDate);

 public Q_SLOTS:
  void updateMaxDayCalendar();
};

#endif  // PRESENTATION_H
