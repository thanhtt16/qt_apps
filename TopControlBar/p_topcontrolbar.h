#ifndef P_TOPCONTROLBAR_H
#define P_TOPCONTROLBAR_H

#include <PacModel/presentation.h>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "Authentication/user.h"
#include "Common/appconfig.h"
#include "Common/appprofile.h"
#include "Common/resources.h"
#include "Site/changesite.h"
#include "Site/sitechild.h"
#include "c_topcontrolbar.h"
#include "message.h"
class C_TopControlBar;
class P_TopControlBar : public Presentation {
public:
    QWidget *zone;
    bool isButtonFullcreenClick = false;

    QWidget *fullScreenWidget = Q_NULLPTR;
    QWidget *brandNameZone = Q_NULLPTR;
    QWidget *siteNameZone = Q_NULLPTR;
    QWidget *appNameZone = Q_NULLPTR;
    QLabel *brandLogo = Q_NULLPTR;
    QLabel *siteIcon = Q_NULLPTR;
    QPushButton *siteName = Q_NULLPTR;
    QPushButton *appName = Q_NULLPTR;
    QLabel *appIcon = Q_NULLPTR;
    QPushButton *profileSetting = Q_NULLPTR;
    QPushButton *btnFullScreen = Q_NULLPTR;
    P_TopControlBar(Control *ctrl, QWidget *zone);
    C_TopControlBar *control() { return (C_TopControlBar *)this->ctrl; }
    void changeControl(Control *ctrl);
    void update();
    void initTopWidget(QWidget *zone);
    void show(QVariant *attachment);
    void hide();
    bool mIsAnimating = true;
    bool mIsShowRight = false;
    int currentApp = 1;
    QPushButton *mAppNameButton = Q_NULLPTR;
    QWidget *siteSelectorZone = Q_NULLPTR;
    QWidget *appSelectorZone = Q_NULLPTR;
    QWidget *profileSettingZone = Q_NULLPTR;
    QWidget *profileSettingWidget = Q_NULLPTR;

    QWidget *getZone(int zoneId);
    QList<QString> mListSiteNameFirst;
    QSize mScreenSize;
    void loadLogoApp();
    void enableSelectedApp();
    void disableSelectedApp();
public Q_SLOTS:
    void onSiteNameClick();
    void onAppNameClick();
    void onProfileSettingClick();

    void onPageControlClick();
    void onLayoutControlClick();
    void siteChanged();
    void appChanged();
    void hideElementTop();

    void enterFullscreenMode();
    void exitFullscreenMode();
    void clickFullSreen();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif  // P_TOPCONTROLBAR_H
