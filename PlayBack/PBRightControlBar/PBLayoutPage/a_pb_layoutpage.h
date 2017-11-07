#ifndef A_PBPAGELAYOUTTAB_H
#define A_PBPAGELAYOUTTAB_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"

class C_PBPageLayoutTab;

class A_PBLayoutPage : public Abstraction {
  Q_OBJECT
 public:
  A_PBLayoutPage(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_PBPAGELAYOUTTAB_H
