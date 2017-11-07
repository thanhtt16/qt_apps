#ifndef A_SETTINGGENERAL_H
#define A_SETTINGGENERAL_H

#include <QObject>
#include "PacModel/control.h"
class C_SettingGeneral;
class A_SettingGeneral : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_SettingGeneral(Control *ctrl);
  C_SettingGeneral *control() { return (C_SettingGeneral *)this->ctrl; }
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
