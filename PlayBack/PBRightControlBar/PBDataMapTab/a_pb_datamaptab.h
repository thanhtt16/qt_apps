#ifndef A_PBDATAMAPTAB_H
#define A_PBDATAMAPTAB_H

#include <QObject>
#include "PacModel/control.h"
class C_PBDataMapTab;
class A_PBDataMapTab : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_PBDataMapTab(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_PBDATAMAPTAB_H
