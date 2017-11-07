#ifndef A_Cam9RTCPlayer_H
#define A_Cam9RTCPlayer_H

#include <QObject>
#include "PacModel/control.h"

class C_Cam9RTCPlayer;
class A_Cam9RTCPlayer : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_Cam9RTCPlayer(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_Cam9RTCPlayer_H
