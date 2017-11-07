#ifndef A_SETTINGS_H
#define A_SETTINGS_H

#include <QObject>
#include "PacModel/control.h"
#include "c_settings.h"

class C_Settings;
class A_Settings : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_Settings(Control *ctrl);
  C_Settings *control() { return (C_Settings *)this->ctrl; }
  void changeControl(Control *ctrl);
};

#endif  // A_SETTINGS_H
