#ifndef A_VWLAYOUTSELECTOR_H
#define A_VWLAYOUTSELECTOR_H

#include <QObject>
#include "PacModel/control.h"

class C_VWLayoutSelector;
class A_VWLayoutSelector : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_VWLayoutSelector(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_VWLayoutSelector_H
