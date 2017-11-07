#ifndef P_PlayBack_H
#define P_PlayBack_H

#include <PacModel/presentation.h>
#include <QObject>
#include "Common/resources.h"
#include "PlayBack/c_playback.h"

class C_PlayBack;
class P_PlayBack : public Presentation {
  // init ui control
 private:
 public:
  QHBoxLayout *layout = Q_NULLPTR;
  QWidget *rightBar = Q_NULLPTR;
  QWidget *workSpace = Q_NULLPTR;

  P_PlayBack(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void show(QWidget *zone);
  void update();

  QWidget *getZone(int zoneId);
  void showRightBar();
  float rightBarPercent = 0.2;

  C_PlayBack *control() { return (C_PlayBack *)ctrl; }

  void siteChanged();
  void enterFullscreenMode();
  void exitFullscreenMode();
};

#endif  // PRESENTATION_H
