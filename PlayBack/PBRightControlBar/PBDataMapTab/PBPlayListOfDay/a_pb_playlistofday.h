#ifndef A_PBPLAYLISTOFDAY_H
#define A_PBPLAYLISTOFDAY_H

#include <QObject>
#include "PacModel/control.h"
class C_PBPlayListOfDay;
class A_PBPlayListOfDay : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_PBPlayListOfDay(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_PBPLAYLISTOFDAY_H
