#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QSettings>
#include "Authentication/userapp.h"
#include "Camera/camsite.h"
#include "Site/site.h"
#include "Site/sitechild.h"
#include "message.h"
#include "sessionuser.h"
#include "user.h"
#include <QSettings>

class QSettings;
class AppContext : public QObject {
    Q_OBJECT
private:
    AppMessage Message;
    User *workingUser = Q_NULLPTR;
    Site *workingSite = Q_NULLPTR;
    Site *siteOfWorker = Q_NULLPTR;
    CamSite *siteCameras = Q_NULLPTR;
    QWidget *mainWindow = Q_NULLPTR;
    SiteChild *userSites = Q_NULLPTR;
    QList<UserApp> userApps;
    UserApp workingApp;
    CamItemType networkType;


    // main application
    QString localStoragePath = "Downloads";  // thu muc media

    // worker site settings (Settings/workerSiteID.txt):
    // + save workingSite
    // + save selectedLayout
    // + save selectedPage
    // + selectedDate
    // + selectedTimeSlot (hour)
    // + selectedPosition (timestamp)
    // + VideoWall app default setting
    int videoWallPageTransitionStartMode =
            Message.PAGE_TRANSITION_START_ON_CLICK; //PAGE_TRANSITION_START_AUTOMATICALLY

    int videoWallPageTransitionDelay = 0;  // 5 minute;

    int totalCameras;
    QNetworkAccessManager *networkManager;
    QList<CamItem *> listCamItemsOfSite;

public:

    AppContext();
    Site *getSiteOfWorker() const;
    void setSiteOfWorker(Site *value);

    Site *getWorkingSite() const;
    void setWorkingSite(Site *value);

    User *getWorkingUser() const;
    void setWorkingUser(User *value);
    void loadWorkingSite();

    QWidget *getMainWindow() const;
    void setMainWindow(QWidget *value);

    CamSite *getSiteCameras() const;
    void setSiteCameras(CamSite *value);

    SiteChild *getUserSites() const;
    void setUserSites(SiteChild *value);

    QList<UserApp> getUserApps() const;
    void setUserApps(const QList<UserApp> &value);

    UserApp getWorkingApp() const;
    void setWorkingApp(const UserApp &value);
    void setWorkingApp(QString appName);

    int getVideoWallPageTransitionStartMode() const;
    void setVideoWallPageTransitionStartMode(int value);

    int getVideoWallPageTransitionDelay() const;
    void setVideoWallPageTransitionDelay(int value);

    QString getLocalStoragePath() const;
    void setLocalStoragePath(const QString &value);

    int getTotalCameras() const;
    void setTotalCameras(int value);

    QList<CamItem *> getListCamItemsOfSite() const;
    void setListCamItemsOfSite(const QList<CamItem *> &value);

    QSettings getAppSettings() const;
    int getMaxHistorydayOfSite();



    CamItemType getNetworkType() const;
    void setNetworkType(const CamItemType &value);

public Q_SLOTS:
};

#endif  // APPCONTEXT_H
