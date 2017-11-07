#ifndef P_SETTINGVIDEOWALL_H
#define P_SETTINGVIDEOWALL_H

#include <PacModel/presentation.h>
#include <QComboBox>
#include <QObject>
#include <QWidget>
#include "c_setting_videowall.h"
class C_SettingVideoWall;
class P_SettingVideoWall : public Presentation {
  Q_OBJECT
 private:
  QWidget *zone;

 public:
  P_SettingVideoWall(Control *ctrl, QWidget *zone);
  C_SettingVideoWall *control() { return (C_SettingVideoWall *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  QComboBox *comboBoxTransition;
  QComboBox *comboBoxDelay;

  QObject *getZone(int zoneId);

 public Q_SLOTS:
  void selectedComboBoxDelay(int index);
  void selectedTransition(int index);
};

#endif  // P_SETTINGVIDEOWALL_H
