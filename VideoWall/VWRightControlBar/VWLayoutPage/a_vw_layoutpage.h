#ifndef A_VWPAGELAYOUTTAB_H
#define A_VWPAGELAYOUTTAB_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"

class C_VWPageLayoutTab;

class A_VWLayoutPage : public Abstraction {
  Q_OBJECT
 public:
  A_VWLayoutPage(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_VWPAGELAYOUTTAB_H
