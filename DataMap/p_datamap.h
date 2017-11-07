#ifndef P_DATAMAP_H
#define P_DATAMAP_H

#include <PacModel/presentation.h>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QVBoxLayout>
#include <QWidget>
#include "DataMap/seekbarwidget.h"

class C_DataMap;

class P_DataMap : public Presentation {
 private:

  SeekBarWidget *seekBarWidget;
  QWidget *zone = Q_NULLPTR;
  bool isMain = false;
  int mainStartTime = 0;
  int mainEndTime = 0;
  int mainTtimeLength = 0;
  bool isPainting = false;

  QVBoxLayout *layout;
  QLabel *startTimeLabel;
  QLabel *endTimeLabel;

 public:
  P_DataMap(Control *ctrl, QWidget *zone);
  C_DataMap *control() { return (C_DataMap *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();

//  void drawDataMapArray(int startTimestamp,int timeDuration);
  QObject *getZone(int zoneId);

  bool getIsMain() const;
  void setIsMain(bool value);
};

#endif  // P_DATAMAP_H
