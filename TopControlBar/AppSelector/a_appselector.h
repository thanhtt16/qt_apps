#ifndef A_APPSELECTOR_H
#define A_APPSELECTOR_H

#include <QObject>
#include "PacModel/control.h"
class C_AppSelector;
class A_AppSelector : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_AppSelector(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_APPSELECTOR_H
