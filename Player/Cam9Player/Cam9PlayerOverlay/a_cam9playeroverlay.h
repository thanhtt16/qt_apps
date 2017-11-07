#ifndef A_CAM9PLAYEROVERLAY_H
#define A_CAM9PLAYEROVERLAY_H

#include <QObject>
#include "PacModel/control.h"
class C_Cam9PlayerOverlay;
class A_Cam9PlayerOverlay : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_Cam9PlayerOverlay(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_CAM9PLAYEROVERLAY_H
