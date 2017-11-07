#ifndef C_SETTINGGENERAL_H
#define C_SETTINGGENERAL_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"
#include "a_setting_general.h"
#include "message.h"
#include "p_setting_general.h"
#include "Authentication/appcontext.h"

class P_SettingGeneral;
class A_SettingGeneral;
class C_Settings;
class C_SettingGeneral : public Control {
  QWidget* zone;

 public:
  AppContext* appContext;
  C_SettingGeneral(Control* ctrl, QWidget* zone);
  C_Settings* getParent() { return (C_Settings*)this->parent; }

  P_SettingGeneral* presenation() { return (P_SettingGeneral*)this->pres; }
  A_SettingGeneral* abstraction() { return (A_SettingGeneral*)this->abst; }
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
};

#endif  // C_SETTINGGENERAL_H
