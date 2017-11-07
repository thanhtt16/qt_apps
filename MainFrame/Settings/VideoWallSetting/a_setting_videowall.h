#ifndef A_SETTINGVIDEOWALL_H
#define A_SETTINGVIDEOWALL_H

#include <QObject>
#include "PacModel/control.h"
#include "c_setting_videowall.h"

class C_SettingVideoWall;
class A_SettingVideoWall : public Abstraction {
  Q_OBJECT
 private:
 public:
  A_SettingVideoWall(Control *ctrl);
  C_SettingVideoWall *control() { return (C_SettingVideoWall *)this->ctrl; }
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
