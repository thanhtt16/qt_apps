#ifndef A_CLASSNAME_H
#define A_CLASSNAME_H

#include <QObject>
#include "PacModel/control.h"
class C_ClassName;
class A_ClassName : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_ClassName(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
