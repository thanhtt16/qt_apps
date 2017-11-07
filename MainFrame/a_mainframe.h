#ifndef A_MAINFRAME_H
#define A_MAINFRAME_H

#include <QMutex>
#include <QObject>
#include <QThreadPool>
#include <functional>
#include "Authentication/connector.h"
#include "Authentication/user.h"
#include "Authentication/userapp.h"
#include "Camera/camsite.h"
#include "Common/networkutils.h"
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "Site/changesite.h"
#include "Site/sitechild.h"
#include "c_mainframe.h"
#include <QSettings>
class C_MainFrame;
class AppInfo;
class A_MainFrame : public Abstraction {
    Q_OBJECT
    // A ref on the control facet
private:
    QList<Site *> userSites;
    CamSite *siteCameras;
    QList<UserApp> userApps;
    bool checkSessionSaved = false;
    LayoutSet *layoutSet = Q_NULLPTR;
    bool loadDataFirstSuccess = false;
    bool loadUserSiteSuccess = false;
    bool isRequestingLoadData = false;
    bool isRequestingLoadUserSite = false;

public:
    QList<QString> loadPlayListM3U8(long timeStampStart, int duration, QList<int> listCamerasId);
    QString convertLinkToM3u8(QString live, long timestamp, int duration);

    A_MainFrame(Control *ctrl);
    void changeControl(Control *ctrl);

    void loadUserSites();
    void loadWorkingSite();

    void loadCamerasOfSiteWithLayoutPage(int pageNumber, int layoutNumber,
                                         std::function<void(void)> onSuccess,
                                         std::function<void(void)> onFailure);

    void loadAllCamerasOfSite(
            std::function<void(void)> onSuccess,
            std::function<void(void)> onFailure);

    void loadTotalCamerasOfSite(int siteId, QString tokenUser, int pageNumber,
                                int layoutNumber,
                                std::function<void(CamSite *)> onSuccess,
                                std::function<void(QString)> onFailure);

    void loadUserApps();

    void changeSite();
    void loadWorkingSiteData();

    void changeApp();

    void getUserSites(QVariant *attachment);
    void getUserApps(QVariant *attachment);

    SiteChild *getChildSite() const;

    C_MainFrame *control();
    CamSite *getSiteCameras() const;
    void setSiteCameras(CamSite *value);
    void onSocketJoined();
    void onSocketDisconnected();
    void onSocketConnected();
    void connectToSocket();
    // DuongNArtist
    //  std::function<void(void)> onLoadDataSuccess;
    //  std::function<void(void)> onLoadDataFailure;
    void loadDataCamerasOfSite();
    void loadDataCamerasWithLayoutPage();
    void reloadDataWhenNetworkIsReachable();
};

#endif  // A_MAINFRAME_H
