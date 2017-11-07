#ifndef A_PBLAYOUTSELECTOR_H
#define A_PBLAYOUTSELECTOR_H

#include <QObject>
#include "PacModel/control.h"

class C_PBLayoutSelector;
class A_PBLayoutSelector : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_PBLayoutSelector(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_PBLayoutSelector_H
