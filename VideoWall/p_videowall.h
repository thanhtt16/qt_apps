#ifndef P_VIDEOWALL_H
#define P_VIDEOWALL_H

#include <PacModel/presentation.h>
#include <QObject>
#include "Common/resources.h"
#include "VideoWall/c_videowall.h"

class C_VideoWall;
class P_VideoWall : public Presentation {
  // init ui control
 private:
 public:
  QWidget *zone = Q_NULLPTR;
  QHBoxLayout *layout = Q_NULLPTR;
  QWidget *rightBar = Q_NULLPTR;
  QWidget *workSpace = Q_NULLPTR;

  P_VideoWall(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void show(QWidget *zone);
  void update();

  QWidget *getZone(int zoneId);
  void showRightBar();
  float rightBarPercent = 0.2;

  C_VideoWall *control() { return (C_VideoWall *)ctrl; }

  void siteChanged();

  void enterFullscreenMode();
  void exitFullscreenMode();
  void pageTransitionBegin();

 protected:
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif  // PRESENTATION_H
