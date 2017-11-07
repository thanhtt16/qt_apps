#ifndef A_PBRecord_H
#define A_PBRecord_H

#include <QObject>
#include "PacModel/control.h"
class C_PBRecord;
class A_PBRecord : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_PBRecord(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
