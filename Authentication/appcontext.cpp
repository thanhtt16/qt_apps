#include "appcontext.h"

Site *AppContext::getSiteOfWorker() const { return siteOfWorker; }

void AppContext::setSiteOfWorker(Site *value) { siteOfWorker = value; }

Site *AppContext::getWorkingSite() const { return workingSite; }



void AppContext::setWorkingSite(Site *value) {
    workingSite = value;
    QSettings settings;
    settings.beginGroup(QString::number(workingUser->getUserId()));
    settings.setValue("id_working_site", workingSite->getSiteId());
    settings.setValue("site_name", workingSite->getSiteName());
    settings.endGroup();
}

User *AppContext::getWorkingUser() const { return workingUser; }

void AppContext::setWorkingUser(User *value) {
    workingUser = value;
}

QWidget *AppContext::getMainWindow() const { return mainWindow; }

void AppContext::setMainWindow(QWidget *value) { mainWindow = value; }

CamSite *AppContext::getSiteCameras() const { return siteCameras; }

void AppContext::setSiteCameras(CamSite *value) { siteCameras = value; }

SiteChild *AppContext::getUserSites() const { return userSites; }

void AppContext::setUserSites(SiteChild *value) { userSites = value; }

QList<UserApp> AppContext::getUserApps() const { return userApps; }

void AppContext::setUserApps(const QList<UserApp> &value) { userApps = value; }

UserApp AppContext::getWorkingApp() const { return workingApp; }

void AppContext::setWorkingApp(const UserApp &value) { workingApp = value; }
void AppContext::setWorkingApp(QString appName) {
    for (int appIndex = 0; appIndex < userApps.size(); ++appIndex) {
        if (userApps.at(appIndex).appName == appName) {
            this->workingApp = userApps.at(appIndex);
            return;
        }
    }
}

int AppContext::getVideoWallPageTransitionStartMode() const {
    return videoWallPageTransitionStartMode;
}

void AppContext::setVideoWallPageTransitionStartMode(int value) {
    videoWallPageTransitionStartMode = value;
}

int AppContext::getVideoWallPageTransitionDelay() const {
    return videoWallPageTransitionDelay;
}

void AppContext::setVideoWallPageTransitionDelay(int value) {
    videoWallPageTransitionDelay = value;
}

QString AppContext::getLocalStoragePath() const { return localStoragePath; }

void AppContext::setLocalStoragePath(const QString &value) {
    localStoragePath = value;
}


int AppContext::getTotalCameras() const { return totalCameras; }

void AppContext::setTotalCameras(int value) { totalCameras = value; }

QList<CamItem *> AppContext::getListCamItemsOfSite() const {
    return listCamItemsOfSite;
}

void AppContext::setListCamItemsOfSite(const QList<CamItem *> &value) {
    listCamItemsOfSite = value;
}


CamItemType AppContext::getNetworkType() const
{
    return networkType;
}

void AppContext::setNetworkType(const CamItemType &value)
{
    networkType = value;
}

AppContext::AppContext() : QObject(Q_NULLPTR) {
    userSites = new SiteChild;
}
void AppContext::loadWorkingSite() {
    // doc tu db ra oldWorkingSite cua phien lam viec lan truoc. neu khong luu thi
    // coi nhu ko co

    // doc tu db ra danh sach site cua user == userSiteList
    // kiem tra xem oldWorkingSite luu trong cau hinh co ton tai trong
    // userSiteList hay khong
    // Neu co thi lay workingSite nay de lam viec this->workingSite =
    // oldWorkingSite;
    // Neu khong co thi lay site cua user lam working site aka this->workingSite =
    // userProfile.site_id;

    //  if (sessionUser) {
    //    Site *site = new Site;
    //    qDebug() << sessionUser->getIdWorkingSite() <<
    //    sessionUser->getSiteName();
    //    site->setSiteId(sessionUser->getIdWorkingSite());
    //    site->setSiteName(sessionUser->getSiteName());
    //    this->workingSite = site;
    //  } else {

    this->workingSite = this->workingUser->getSite();
    //  for (int index = 0; index < this->userSites->getListSite().size();
    //  ++index) {
    //    if (sessionUser->getIdWorkingSite() ==
    //        this->userSites->getListSite().at(index)->getSiteId()) {
    //      this->workingSite = this->userSites->getListSite().at(index);
    //    }
    //  }
}


int AppContext::getMaxHistorydayOfSite(){
    int maxHistoryday = 0;
    int historyDayOfCamItem;
    bool ok;
    CamItem *camItem = Q_NULLPTR;
    if(siteCameras){
        QList<CamItem *> listCamItem = siteCameras->getCamItems();
        //select history day first
        if(listCamItem.size() > 0) {
            camItem = listCamItem.at(0);
            if(camItem){
                historyDayOfCamItem = camItem->getHistoryDay().toInt(&ok);
                if(ok){
                    maxHistoryday = historyDayOfCamItem;
                }
            }
        }
        //find and change maxhistoryday
        for (int index = 1; index < listCamItem.size(); ++index) {
            camItem = listCamItem.at(index);
            if(camItem){
                historyDayOfCamItem = camItem->getHistoryDay().toInt(&ok);
                if(maxHistoryday < historyDayOfCamItem){
                    maxHistoryday = historyDayOfCamItem;
                }
            }
        }
    }
    return maxHistoryday;
}


