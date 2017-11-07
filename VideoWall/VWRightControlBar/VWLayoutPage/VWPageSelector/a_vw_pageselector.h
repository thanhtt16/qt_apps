#ifndef A_VWPageSelector_H
#define A_VWPageSelector_H

#include <QObject>
#include "Authentication/appcontext.h"
#include "PacModel/control.h"

class C_VWPageSelector;
class A_VWPageSelector : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_VWPageSelector(Control *ctrl);
  void changeControl(Control *ctrl);

  AppContext *appContext = Q_NULLPTR;
};

#endif  // A_VWPageSelector_H
