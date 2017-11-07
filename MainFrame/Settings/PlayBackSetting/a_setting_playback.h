#ifndef A_SETTINGPLAYBACK_H
#define A_SETTINGPLAYBACK_H

#include <QObject>
#include "PacModel/control.h"
#include "c_setting_playback.h"

class C_SettingPlayBack;
class A_SettingPlayBack : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_SettingPlayBack(Control *ctrl);
  C_SettingPlayBack *control() { return (C_SettingPlayBack *)this->ctrl; }
  void changeControl(Control *ctrl);
};

#endif  // A_SETTINGPLAYBACK_H
