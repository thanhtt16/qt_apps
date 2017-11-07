#ifndef A_PROFILESETTING_H
#define A_PROFILESETTING_H

#include <QObject>
#include "PacModel/control.h"
class C_ProfileSetting;
class A_ProfileSetting : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_ProfileSetting(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_PROFILESETTING_H
