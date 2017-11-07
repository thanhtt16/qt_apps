#ifndef A_LOGIN_H
#define A_LOGIN_H

#include <QObject>
#include "PacModel/control.h"
class C_Login;
class A_Login : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_Login(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
