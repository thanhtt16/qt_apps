#ifndef P_SETTINGGENERAL_H
#define P_SETTINGGENERAL_H

#include <PacModel/presentation.h>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include "../c_settings.h"
#include "Site/site.h"
#include "c_setting_general.h"
#include <QSettings>
#include <QRadioButton>
#include <QDebug>
class C_SettingGeneral;
class P_SettingGeneral : public Presentation {
    //  Q_OBJECT
private:
    QWidget *zone;

public:
    P_SettingGeneral(Control *ctrl, QWidget *zone);
    C_SettingGeneral *control() { return (C_SettingGeneral *)this->ctrl; }
    QLabel *siteWorkingLabel;
    QLabel *appVersionLabel;
    QLineEdit *workingSiteLineEdit;
    QLineEdit *versionLineEdit;
    QRadioButton *cdnRadioButton;
    QRadioButton *nvrRadioButton;
    QLabel *streamLabel;

    void updateWorkingSite(Site *site);
    QObject *getZone(int zoneId);
public Q_SLOTS:
    void cdnSelected(bool checked);
    void nvrSelected(bool checked);

};

#endif  // P_SETTINGGENERAL_H
