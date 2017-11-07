#ifndef C_SETTINGS_H
#define C_SETTINGS_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "GeneralSetting/c_setting_general.h"
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "PlayBackSetting/c_setting_playback.h"
#include "VideoWallSetting/c_setting_videowall.h"
#include "a_settings.h"
#include "message.h"
#include "p_settings.h"

class C_SettingGeneral;
class C_SettingVideoWall;
class C_SettingPlayBack;

class P_Settings;
class A_Settings;
class C_MainFrame;
class C_Settings : public Control {
  Q_OBJECT
  QWidget* zone;
  C_SettingGeneral* cSettingGeneral;
  C_SettingVideoWall* cSettingVideoWall;
  C_SettingPlayBack* cSettingPlayBack;
  AppContext* appContext;

 public:
  C_Settings(Control* ctrl, QWidget* zone);
  C_MainFrame* getParent() { return (C_MainFrame*)this->parent; }
  A_Settings* abstraction() { return (A_Settings*)this->abst; }
  P_Settings* presentation() { return (P_Settings*)this->pres; }

  /**
        * Method to receive a message from the Presentation Facet.
        * @param message    : A string which describe the request
        * @param attachment : A ref on an eventual object necessary to treat the
    *request
        **/
  void newUserAction(int message, QVariant* attachment) override;
  /**
    * Method to receive a message from the Astraction Facet.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newSystemAction(int message, QVariant* attachment) override;

  /**
    * Method to receive a message from an other agent.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newAction(int message, QVariant* attachment) override;
  AppContext* getAppContext() const;
  void setAppContext(AppContext* value);
};

#endif  // C_SETTINGS_H
