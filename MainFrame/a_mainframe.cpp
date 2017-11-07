#include "MainFrame/a_mainframe.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
CamSite* A_MainFrame::getSiteCameras() const { return siteCameras; }

void A_MainFrame::setSiteCameras(CamSite* value) { siteCameras = value; }

A_MainFrame::A_MainFrame(Control* ctrl) : Abstraction(ctrl) {
    layoutSet = new LayoutSet(Q_NULLPTR);
    this->loadUserApps();
}

void A_MainFrame::connectToSocket() {
    Connector* connector = new Connector();
    connect(connector, &Connector::socketConnected, this,
            &A_MainFrame::onSocketConnected);
    connect(connector, &Connector::socketDisconnected, this,
            &A_MainFrame::onSocketDisconnected);
    connect(connector, &Connector::socketJoined, this,
            &A_MainFrame::onSocketJoined);
    connector->connectToSocket(
                control()->getAppContext()->getWorkingUser()->getToken());
}

void A_MainFrame::onSocketJoined() {
    //  QMutex mutex;
    //  mutex.lock();
    //  loadSiteCameras();
    //  mutex.unlock();
}
void A_MainFrame::onSocketDisconnected() {  }
void A_MainFrame::onSocketConnected() {}
/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_MainFrame::changeControl(Control* ctrl) { this->ctrl = ctrl; }

/**
 * Method to create a new abstraction exactly equals to this abstraction.
 * @return the created clone
 **/

// public Abstraction getClone(){
//    try{
//        return (Abstraction)clone();
//    } catch(Exception e) {System.out.println("ERROR: can't duplicate an
//    abstraction.");}
//    return null;
//}

void A_MainFrame::getUserSites(QVariant* attachment) {
    attachment->setValue(this->userSites);
}

void A_MainFrame::getUserApps(QVariant* attachment) {
    attachment->setValue(this->userApps);
}
// thao Add 24.08.2017
void A_MainFrame::loadTotalCamerasOfSite(
        int siteId, QString tokenUser, int pageNumber, int layoutNumber,
        std::function<void(CamSite*)> onSuccess,
        std::function<void(QString)> onFailure) {
    CamSite::getCamerasOfSiteWithLayoutPage(
                siteId, tokenUser, pageNumber, layoutNumber,
                [this, onSuccess](CamSite* siteCameras) { onSuccess(siteCameras); },
    [this, onFailure](QString message) { onFailure(message); });
}

//load list camera with api 004
void A_MainFrame::loadCamerasOfSiteWithLayoutPage(int pageNumber, int layoutNumber,
                                                  std::function<void(void)> onSuccess,
                                                  std::function<void(void)> onFailure) {
    QSettings settings;
    settings.beginGroup(QString::number(this->control()->getAppContext()->getWorkingUser()->getUserId()));
    int siteIdUserSession = settings.value("id_working_site").toInt();

    int siteId = control()->getAppContext()->getWorkingUser()->getSite()->getSiteId();
    qDebug() << "loadSiteCameras" << "SITE ID SESSION" << siteIdUserSession << "SITE ID LOGIN" <<  siteId;
    QString token = control()->getAppContext()->getWorkingUser()->getToken();
    if (siteIdUserSession != -1 && siteIdUserSession != 0) {
        siteId = siteIdUserSession;
    }

    qDebug() << "load site camera" << "SITE ID" << siteId;

    CamSite::getCamerasOfSiteWithLayoutPage(
                siteId, token, pageNumber, layoutNumber,
                [this, onSuccess](CamSite* siteCameras) {
        qDebug() << Q_FUNC_INFO << siteCameras->getCamItems().size();
        this->siteCameras = siteCameras;
        this->control()->getAppContext()->setSiteCameras(siteCameras);
        this->control()->getAppContext()->setTotalCameras(
                    siteCameras->getTotalCamItem());

        //update maxhistoryday for calendar
        this->control()->newSystemAction(Message.APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY, Q_NULLPTR);

        qDebug() << "TOTAL" <<  siteCameras->getTotalCamItem();
        onSuccess();
    },
    [this, onFailure](QString msg) {
        onFailure(); });
}


//load list camera with api 003
void A_MainFrame::loadAllCamerasOfSite(
        std::function<void(void)> onSuccess,
        std::function<void(void)> onFailure) {


    QSettings settings;
    settings.beginGroup(QString::number(this->control()->getAppContext()->getWorkingUser()->getUserId()));
    int siteIdUserSession = settings.value("id_working_site").toInt();
    int siteId = control()->getAppContext()->getWorkingUser()->getSite()->getSiteId();

    qDebug() << "loadSiteCameras" << "SITE ID SESSION" << siteIdUserSession << "SITE ID LOGIN" <<  siteId;

    QString token = control()->getAppContext()->getWorkingUser()->getToken();

    if (siteIdUserSession != -1 && siteIdUserSession != 0) {
        siteId = siteIdUserSession;
    }
    qDebug() << "load site camera" << "SITE ID" << siteId;

    CamSite::getCamerasOfSite(
                siteId, token,
                [this, onSuccess](CamSite* siteCameras) {

        this->siteCameras = siteCameras;
        this->control()->getAppContext()->setSiteCameras(siteCameras);
        this->control()->getAppContext()->setTotalCameras(
                    siteCameras->getCamItems().size());

        qDebug() << "TOTAL" <<   siteCameras->getCamItems().size();
        onSuccess();
    },
    [this, onFailure](QString msg) { onFailure(); });
}

QString A_MainFrame::convertLinkToM3u8(QString live, long timestamp, int duration) {
    QString vod = "http://";
    QStringList lives = live.split("/");
    vod.append(lives.at(2));
    vod.append("/rec/hls/");
    vod.append(lives.last());
    vod.append("_");
    vod.append(QString::number(timestamp * 1000));
    vod.append("_");
    vod.append(QString::number(duration * 1000));
    vod.append("_h264.m3u8");
    return vod;
}

QList<QString> A_MainFrame::loadPlayListM3U8(long timeStampStart, int duration, QList<int> listCamerasId){
    // để lấy luồng SD thì truyền name = "SD", nếu cần lấy luồng HD thì name = "HD"
    CamItemType typeItem;
    typeItem.protocol = "WS";
    typeItem.network = "CDN";
    typeItem.name = "SD"; // nếu muốn lấy luồng hd thì value = "HD"

    QList<QString> playlistM3U8;
    if(this->siteCameras != Q_NULLPTR)
    {
        QList<CamItem * > listCamItems = this->siteCameras->getCamItems();
        QList<int> camrerasId;
        for (int index = 0; index < listCamItems.size(); ++index) {
            camrerasId.append(listCamItems.at(index)->getCameraId());
        }
        for (int index = 0; index < listCamerasId.size(); ++index) {
            int indexOfCameraId = camrerasId.indexOf(listCamerasId.at(index)); //lấy chỉ số của camraid tương ứng trong list danh sách cameras id load về
            if(indexOfCameraId == -1) {
                playlistM3U8.append(""); //nếu không tìm thấy thì add giá trị xâu rỗng
            }else{
                //nếu khác -1 tức camera id này tồn tại trong list danh sách load về
                //bắt đầu lấy link
                CamItem *camItem = listCamItems.at(indexOfCameraId);
                if(camItem != Q_NULLPTR){
                    CamStream *camStream = camItem->getCamStream(typeItem);
                    if(camStream != Q_NULLPTR){
                        //lay source live tuong ung cua tung cameraid
                        QString sourceLiveOfCamStream = camStream->getSource();
                        //chuyen doi link live thanh link m3u8
                        QString sourceM3u8OfCamStream = convertLinkToM3u8(sourceLiveOfCamStream, timeStampStart, duration);
                        playlistM3U8.append(sourceM3u8OfCamStream); //thêm source vào list trả về
                    }
                }
            }
        }
    }
    return playlistM3U8;
}


void A_MainFrame::loadUserSites() {
    isRequestingLoadUserSite = true;
    AppContext* appContext = control()->getAppContext();
    SiteChild::fetchListSiteChild(
                appContext->getWorkingUser()->getSite()->getSiteId(),
                appContext->getWorkingUser()->getToken(),
                [this](SiteChild* userSites) {
        this->control()->getAppContext()->getUserSites()->clearListSite();
        this->control()->getAppContext()->setUserSites(userSites);
        this->control()->newSystemAction(Message.APP_UPDATE_USER_SITES, Q_NULLPTR);
        loadUserSiteSuccess = true;
        isRequestingLoadUserSite = false;
        if(loadUserSiteSuccess && loadDataFirstSuccess){
            control()->newAction(Message.STOP_TIMER_RECHECK_NETWORK, Q_NULLPTR);
        }
    },
    [this](QString message) {
        loadUserSiteSuccess = false;
        isRequestingLoadUserSite = false;
    });
}

void A_MainFrame::loadUserApps() {
    this->userApps.append({"Video Wall", 1, ""});
    this->userApps.append({"Playback", 2, ""});
    this->control()->getAppContext()->setUserApps(userApps);
    //  control()->newSystemAction(Message.DISPLAY_LIST_APPS, Q_NULLPTR);
}

void A_MainFrame::loadWorkingSite() {}

void A_MainFrame::loadWorkingSiteData() {
    qDebug() << Q_FUNC_INFO;
    loadCamerasOfSiteWithLayoutPage(1, 4, [this] {  /*this->onLoadDataSuccess();*/ },
    [this] { /*this->onLoadDataFailure();*/ });
    //  loadUserSites();
}

void A_MainFrame::changeSite() { loadWorkingSiteData(); }

void A_MainFrame::changeApp() {}

C_MainFrame* A_MainFrame::control() { return ((C_MainFrame*)this->ctrl); }

//api 004
void A_MainFrame::loadDataCamerasWithLayoutPage() {
    isRequestingLoadData = true;
    int defaultLayoutIndex = 1;
    int defaultPageIndex = 1;
    int selectedLayoutSaved = -1;
    int selectedPageSaved = -1;
    int numberLayout = 4;

    QSettings settings;
    settings.beginGroup(QString::number(this->control()->getAppContext()->getWorkingUser()->getUserId()));
    settings.beginGroup("videowall");
    selectedLayoutSaved = settings.value("selected_layout").toInt();
    selectedPageSaved = settings.value("selected_page").toInt();

    qDebug() << "SETTINGS USER READ" << selectedLayoutSaved << selectedPageSaved;
    //select layout
    if(selectedLayoutSaved >= 0 && defaultLayoutIndex <= 4){
        defaultLayoutIndex = selectedLayoutSaved;
        numberLayout = layoutSet->layoutList.at(defaultLayoutIndex).numberOfCameras;
    }

    if (selectedPageSaved > 0){
        defaultPageIndex = selectedPageSaved;
    }

    qDebug() << "SETTINGS LAYOUT/PAGE Load " << selectedLayoutSaved << selectedPageSaved;

    loadCamerasOfSiteWithLayoutPage(defaultPageIndex,numberLayout , [this] {
        control()->newSystemAction(Message.APP_DATA_LOADED_SUCCESS, Q_NULLPTR);
        control()->newSystemAction(Message.APP_VIDEO_WALL_LAYOUT_DEFAULT_SET,
                                   Q_NULLPTR);
        control()->newSystemAction(Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET,
                                   Q_NULLPTR);
        loadDataFirstSuccess = true;
        isRequestingLoadData = false;
        if(loadUserSiteSuccess && loadDataFirstSuccess){
            control()->newAction(Message.STOP_TIMER_RECHECK_NETWORK, Q_NULLPTR);
        }
    },
    [this] {
        loadDataFirstSuccess = false;
        isRequestingLoadData = false;
        qDebug() << "LOAD SITE CAMERAS ERROR";
    });
}


void A_MainFrame::reloadDataWhenNetworkIsReachable(){
    qDebug() <<"reloadDataWhenNetworkIsReachable";
    if(!isRequestingLoadData && loadDataFirstSuccess == false) {
        loadDataCamerasWithLayoutPage();
    }
    if(!isRequestingLoadUserSite && loadUserSiteSuccess == false){
       loadUserSites();
    }
}

//api 003
void A_MainFrame::loadDataCamerasOfSite() {
    loadAllCamerasOfSite([this] {
        control()->newSystemAction(Message.APP_DATA_LOADED_SUCCESS, Q_NULLPTR);
    },
    [this] {
        qDebug() << "LOAD SITE CAMERAS ERROR";
    });
}


