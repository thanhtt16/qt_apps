#ifndef C_SETTINGVIDEOWALL_H
#define C_SETTINGVIDEOWALL_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "MainFrame/Settings/c_settings.h"
#include "PacModel/control.h"
#include "a_setting_videowall.h"
#include "message.h"
#include "p_setting_videowall.h"
class P_SettingVideoWall;
class A_SettingVideoWall;
class C_Settings;
class C_SettingVideoWall : public Control {
    Q_OBJECT
    AppContext* appContext;
public:
    QWidget* zone;
    C_SettingVideoWall(Control* ctrl, QWidget* zone);
    C_Settings* getParent() { return (C_Settings*)this->parent; }

    A_SettingVideoWall* abstraction() { return (A_SettingVideoWall*)this->abst; }
    P_SettingVideoWall* presentation() { return (P_SettingVideoWall*)this->pres; }
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
    AppContext *getAppContext() const;
};

#endif  // C_SETTINGVIDEOWALL_H
