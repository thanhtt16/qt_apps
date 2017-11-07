#ifndef A_CAM9PLAYER_H
#define A_CAM9PLAYER_H

#include <QObject>
#include "PacModel/control.h"

class C_Cam9Player;
class A_Cam9Player : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_Cam9Player(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_CAM9PLAYER_H
