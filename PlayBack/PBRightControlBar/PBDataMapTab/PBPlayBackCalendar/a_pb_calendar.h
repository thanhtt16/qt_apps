#ifndef A_PBCAlENDAR_H
#define A_PBCAlENDAR_H

#include <QObject>
#include "PacModel/control.h"
class C_PBCalendar;
class A_PBCalendar : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_PBCalendar(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
