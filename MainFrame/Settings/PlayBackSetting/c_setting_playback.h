#ifndef C_SettingPlayBack_H
#define C_SettingPlayBack_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "../c_settings.h"
#include "PacModel/control.h"
#include "a_setting_playback.h"
#include "message.h"
#include "p_setting_playback.h"
class C_Settings;
class P_SettingPlayBack;
class A_SettingPlayBack;
class C_SettingPlayBack : public Control {
  QWidget* zone;

 public:
  AppContext* appContext;
  C_SettingPlayBack(Control* ctrl, QWidget* zone);
  C_Settings* getParent() { return (C_Settings*)this->parent; }
  P_SettingPlayBack* presentation() { return (P_SettingPlayBack*)this->pres; }
  A_SettingPlayBack* abstraction() { return (A_SettingPlayBack*)this->abst; }
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

#endif  // CONTROL_H
