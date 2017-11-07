#ifndef A_PBPageSelector_H
#define A_PBPageSelector_H

#include <QObject>
#include "Authentication/appcontext.h"
#include "PacModel/control.h"

class C_PBPageSelector;
class A_PBPageSelector : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_PBPageSelector(Control *ctrl);
  void changeControl(Control *ctrl);

  AppContext *appContext = Q_NULLPTR;
};

#endif  // A_PBPageSelector_H
