#ifndef A_Cam9RTCPlayerOverlay_H
#define A_Cam9RTCPlayerOverlay_H

#include <QObject>
#include "PacModel/control.h"
class C_Cam9RTCPlayerOverlay;
class A_Cam9RTCPlayerOverlay : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_Cam9RTCPlayerOverlay(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_Cam9RTCPlayerOverlay_H
