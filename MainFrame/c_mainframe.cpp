#include "c_mainframe.h"
/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
AppContext *C_MainFrame::getAppContext() { return appContext; }

void C_MainFrame::setAppContext(AppContext *value) { appContext = value; }

C_MainFrame::C_MainFrame(Control *ctrl, QWidget *zone) : Control(ctrl) {
    this->zone = zone;
    cLogin = new C_Login(this, Q_NULLPTR);
    QVariant *dataStruct = new QVariant();
    this->newAction(Message.APP_LOGIN_SHOW, dataStruct);
    User *user = dataStruct->value<User *>();  // login failed then quit

    if (user == Q_NULLPTR) {
        qApp->exit();
        QProcess process;
        process.execute(QString("kill %1").arg(
                            QApplication::QCoreApplication::applicationPid()));
    } else {
        QSettings settings;
        appContext = new AppContext();
        //cdn type default
        CamItemType typeCdnDefault;
        typeCdnDefault.protocol = "WS";
        typeCdnDefault.network = "LAN";
        //find cdn type in file session user
        QString protocolSaved = settings.value("protocol_type").toString();
        QString networkSaved = settings.value("network_type").toString();

        if((protocolSaved == "WS" && networkSaved == "LAN") || (protocolSaved == "WS" && networkSaved == "CDN")){
            //neu truoc do da lua chon
            typeCdnDefault.protocol = protocolSaved;
            typeCdnDefault.network = networkSaved;
        }else{
            settings.setValue("protocol_type","WS");
            settings.setValue("network_type","LAN");
        }

        //set cdn type working to appcontext
        appContext->setNetworkType(typeCdnDefault);
        appContext->setWorkingUser(user);
        this->appContext->getUserSites()->appendListSiteChild(user->getSite());
        appContext->setMainWindow(zone);

        settings.beginGroup(QString::number(user->getUserId())); //tim xem co ton tai thong tin cua id nay chua
        QStringList listKey = settings.childKeys();
        qDebug() << "LIST KEY " << listKey;
        if(listKey.size() == 0){
            //chưa có SessionUser của nguoi dung
            AppSetttings::instance().createSettingsDefaults(user->getUserId());
            appContext->setSiteOfWorker(user->getSite());
            appContext->setWorkingSite(user->getSite());
        }else{
            //neu co
            int siteIdUserSession = settings.value("id_working_site").toInt();
            QString siteName = settings.value("site_name").toString();
            if (siteIdUserSession != -1 && siteName != "") {
                //neu ton tai siteid cu cua nguoi dung
                Site *siteSession = new Site();
                siteSession->setSiteId(siteIdUserSession);
                siteSession->setSiteName(siteName);
                appContext->setSiteOfWorker(siteSession);
                appContext->setWorkingSite(siteSession);
            }
        }

        //version app settings
        settings.setValue("app_version", "3.11.7");
        settings.setValue("date_build", "15:00 07/11/2017 (GMT+7)");
        this->pres = new P_MainFrame(this, this->zone);
        this->abst = new A_MainFrame(this);

        cTopControlBar = new C_TopControlBar(this, presentation()->getZone(0));
        cTopControlBarOverlay =
                new C_TopControlBar(this, presentation()->getZone(-1));

        cVideoWall = new C_VideoWall(this, presentation()->getZone(1));
        cPlayBack = new C_PlayBack(this, presentation()->getZone(2));
        cSettings = new C_Settings(this, presentation()->getZone(5));


        // init socket
        cSocket = new C_Socket(this, this->presentation()->getZone(6));
        cSocket->newAction(Message.APP_SOCKET_TO_CONNECT, Q_NULLPTR);

        cNetwork = new C_Network(this, this->presentation()->getZone(6));
        cNetwork->newAction(Message.APP_NETWORK_START_CHECKING, Q_NULLPTR);

        cAboutApp = new C_About(this, this->presentation()->getZone(7));
        // this->zone->showFullScreen();
        // newAction(Message.APP_PLAY_BACK_SHOW, 0);

        //      newAction(Message.APP_VIDEO_WALL_SHOW, 0);
        //      newAction(Message.SHOW_INDICATOR, Q_NULLPTR);
        //      cTopControlBarOverlay->newAction(Message.SHOW_INDICATOR,Q_NULLPTR);
        //      cTopControlBar->newAction(Message.SHOW_INDICATOR,Q_NULLPTR);

        // load data
        this->abstraction()->loadDataCamerasWithLayoutPage();
        this->abstraction()->loadUserSites();
    }
    currentDateSelected = QDateTime::currentDateTime();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &C_MainFrame::timeUpdate);
    timer->start(1000);
}

void C_MainFrame::timeUpdate() {
    QString day1 = currentDateSelected.date().toString("dd:MM:yyyy");
    QDateTime currentDate = QDateTime::currentDateTime();
    QString day2 = currentDate.date().toString("dd:MM:yyyy");
    countTimePlayListOfDay++;
    countTimeDelayTransition++;
    if (day1 != day2) {
        cPlayBack->newAction(Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR,
                             Q_NULLPTR);
    }

    if (countTimePlayListOfDay == ((60 - currentDate.time().minute()) * 1000)) {
        countTimePlayListOfDay = 0;
        cPlayBack->newAction(Message.APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY,
                             Q_NULLPTR);
    }
    //default
    modePageTransition = Message.PAGE_TRANSITION_START_ON_CLICK;
    timeDelayTrasition = 300;

    QSettings settings;
    settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
    int timeTransitionDelaySave = settings.value("page_transition_delay").toInt();
    int modePageTransitionSave = settings.value("page_transition_start_mode").toInt();
    if(timeTransitionDelaySave > 0){
        timeDelayTrasition = timeTransitionDelaySave;
    }

    if(modePageTransitionSave >0){
        modePageTransition = modePageTransitionSave ;
    }

    if ((countTimeDelayTransition == timeDelayTrasition) && (modePageTransition == Message.PAGE_TRANSITION_START_AUTOMATICALLY)) {
        if (cVideoWall != Q_NULLPTR) {
            countTimeDelayTransition = 0;
            qDebug() << "PAGE_TRANSITION_BEGIN ";
            cVideoWall->newAction(Message.PAGE_TRANSITION_BEGIN, Q_NULLPTR);
        }
    }
    if (this->appContext->getVideoWallPageTransitionStartMode() ==
            Message.PAGE_TRANSITION_START_ON_CLICK) {
        countTimeDelayTransition = 0;
    }

}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_MainFrame::newUserAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.TOP_CONTROL_BAR_GET_SITE_LIST: {
        if (cTopControlBar != Q_NULLPTR) {
            cTopControlBar->newUserAction(Message.TOP_CONTROL_BAR_GET_SITE_LIST,
                                          attachment);
        }
        if (cTopControlBarOverlay != Q_NULLPTR) {
            cTopControlBarOverlay->newUserAction(
                        Message.TOP_CONTROL_BAR_GET_SITE_LIST, attachment);
        }
    } break;

    case Message.ENTER_FULLSCREEN_MODE: {
        if (cVideoWall != Q_NULLPTR) {
            cVideoWall->newAction(message, attachment);
        }
        if (cPlayBack != Q_NULLPTR) {
            cPlayBack->newAction(message, attachment);
        }
    } break;

    case Message.EXIT_FULLSCREEN_MODE: {
        if (cVideoWall != Q_NULLPTR) {
            cVideoWall->newAction(message, attachment);
        }
        if (cPlayBack != Q_NULLPTR) {
            cPlayBack->newAction(message, attachment);
        }
    } break;

    default:
        qDebug() << "ERROR : General User action in" << getClass()
                 << "non-catched :" << Message.toString(message);
        break;
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_MainFrame::newSystemAction(int message, QVariant *attachment) {
    switch (message) {

    case Message.APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY:{
        cPlayBack->newAction(message, attachment);
    }break;

    case Message.APP_UPDATE_USER_SITES: {
        cTopControlBar->newAction(message, attachment);
        cTopControlBarOverlay->newAction(message, attachment);

    } break;

    case Message.APP_DATA_LOADED_SUCCESS:{
        qDebug() << "APP_DATA_LOADED_SUCCESS";

        newAction(Message.APP_VIDEO_WALL_SHOW, 0);
        newAction(Message.ENTER_FULLSCREEN_MODE, 0);

    } break;

    case Message.APP_VIDEO_WALL_LAYOUT_DEFAULT_SET: {
        if (cVideoWall != Q_NULLPTR) {
            cVideoWall->newAction(message, attachment);
        }
    } break;

    case Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET: {
        if (cPlayBack != Q_NULLPTR) {
            cPlayBack->newAction(message, attachment);
        }
    } break;

    case Message.DISPLAY_LIST_APPS: {
        if (cTopControlBar != Q_NULLPTR) {
            cTopControlBar->newAction(message, attachment);
        }
    } break;

    default:
        qDebug() << "ERROR : General System action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_MainFrame::newAction(int message, QVariant *attachment) {
    switch (message) {
    case  Message.STOP_TIMER_RECHECK_NETWORK:{
        cNetwork->newAction(message, attachment);
    }break;


    case Message.UPDATE_CDN_TYPE_SELECTED:{
        cPlayBack->newAction(message,attachment);
        cVideoWall->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_ALL_CLIP_RECORD_SUCCESS:{
        cPlayBack->newAction(message, attachment);
    }
        break;
    case Message.APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS:{
        cPlayBack->newAction(message, attachment);
    }
        break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD:{
        cSocket->newAction(message, attachment);
    }break;

    case Message.APP_SHOW_SETTINGS: {
        this->cSettings->newAction(message, attachment);
    } break;

    case Message.APP_HIDE_SETTINGS: {
        this->cSettings->newAction(message, attachment);
    } break;

    case Message.LOGOUT: {
        User::clearUserInfo();
        qApp->exit();
    } break;

    case Message.EXIT_APP: {
        qApp->exit();
    } break;

    case Message.SITE_NEW_SELECTED: {
        Site *site = attachment->value<Site *>();
        getAppContext()->setWorkingSite(site);
        abstraction()->changeSite();
        cTopControlBarOverlay->newAction(Message.SITE_CHANGED, attachment);
        cTopControlBar->newAction(Message.SITE_CHANGED, attachment);
        cVideoWall->newAction(Message.SITE_CHANGED, attachment);
        cPlayBack->newAction(Message.SITE_CHANGED, attachment);
        cSettings->newAction(Message.SITE_CHANGED, attachment);
    } break;

    case Message.APP_NEW_SELECTED: {
        abstraction()->changeApp();
        cTopControlBarOverlay->newAction(Message.APP_CHANGED, attachment);
        cTopControlBar->newAction(Message.APP_CHANGED, attachment);
        cVideoWall->newAction(Message.APP_CHANGED, attachment);
        cPlayBack->newAction(Message.APP_CHANGED, attachment);
    } break;

    case Message.PROJECT_RUN:
        this->zone->show();
        break;

    case Message.APP_LOGIN_SHOW:
        cLogin->newAction(Message.APP_LOGIN_SHOW, attachment);
        break;

    case Message.APP_VIDEO_WALL_SHOW: {
        qDebug() << "APP_VIDEO_WALL_SHOW";

        presentation()->showApp(message);
        cVideoWall->newAction(Message.APP_SHOW, attachment);
        this->appContext->setWorkingApp("Video Wall");

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<QString>("Video Wall");
        //clear source change_app
        cPlayBack->newAction(Message.APP_CHANGED, dataStruct);
        cTopControlBarOverlay->newAction(Message.APP_CHANGED, dataStruct);
        cTopControlBar->newAction(Message.APP_CHANGED, dataStruct);

        QVariant *setingData = new QVariant();
        setingData->setValue(1);
        this->cSettings->newAction(Message.APP_UPDATE_INDEX_SETTINGS, setingData);
    } break;

    case Message.APP_PLAY_BACK_SHOW: {
        presentation()->showApp(message);
        cPlayBack->newAction(Message.APP_SHOW, attachment);
        this->appContext->setWorkingApp("Playback");

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<QString>("Playback");
        //clear source change_app
        cVideoWall->newAction(Message.APP_CHANGED, dataStruct);
        cTopControlBarOverlay->newAction(Message.APP_CHANGED, dataStruct);
        cTopControlBar->newAction(Message.APP_CHANGED, dataStruct);

        QVariant *setingData = new QVariant();
        setingData->setValue(2);
        this->cSettings->newAction(Message.APP_UPDATE_INDEX_SETTINGS, setingData);
    } break;

    case Message.APP_MAGIC_SHOW_SHOW: {
        presentation()->showApp(message);
        cMagicShow->newAction(Message.APP_SHOW, attachment);
    } break;

    case Message.APP_CONTEXT_GET:
        attachment->setValue(appContext);
        break;

    case Message.APP_CAM_SITE_GET: {
        attachment->setValue(appContext);
    } break;

    case Message.ENTER_FULLSCREEN_MODE: {
        presentation()->enterFullscreenMode();
    } break;

    case Message.TOP_BAR_OVER_LAY_CAN_HIDE: {
        presentation()->topBarOverlayCanHide();
    } break;

    case Message.TOP_BAR_OVER_LAY_CAN_NOT_HIDE: {
        presentation()->topBarOverlayCanNotHide();
    } break;

    case Message.EXIT_FULLSCREEN_MODE: {
        presentation()->exitFullscreenMode();
    } break;

    case Message.APP_PLAY_BACK_SET_PATH_SAVE_MEDIA: {
        QString pathSaveMedia = attachment->value<QString>();
        QSettings settings;
        //        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.setValue("path_save_media",pathSaveMedia );

    } break;

    case Message.PAGE_TRANSITION_START_ON_CLICK: {
        this->appContext->setVideoWallPageTransitionStartMode(
                    Message.PAGE_TRANSITION_START_ON_CLICK);
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.setValue("page_transition_start_mode",Message.PAGE_TRANSITION_START_ON_CLICK);

    } break;

    case Message.PAGE_TRANSITION_START_AUTOMATICALLY: {
        this->appContext->setVideoWallPageTransitionStartMode(
                    Message.PAGE_TRANSITION_START_AUTOMATICALLY);
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.setValue("page_transition_start_mode",Message.PAGE_TRANSITION_START_AUTOMATICALLY);
    } break;

    case Message.PAGE_TRANSITION_DELAY: {
        int pageTransitionDelay = attachment->value<int>();
        this->appContext->setVideoWallPageTransitionDelay(pageTransitionDelay);
        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.setValue("page_transition_delay",pageTransitionDelay);
    } break;

    case Message.APP_SOCKET_CONNECTED:{
        //        this->abstraction()->loadDataCamerasWithLayoutPage();
    }break;

    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        LayoutStruct layoutStruct = attachment->value<LayoutStruct>();
        int pageNumber = layoutStruct.selectedPage;
        int layoutNumber = layoutStruct.numberOfCameras;
        this->abstraction()->loadCamerasOfSiteWithLayoutPage(pageNumber, layoutNumber, [this]{
            cPlayBack->newAction(Message.APP_PLAY_BACK_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS, Q_NULLPTR);
        }, [] {
            qDebug() << "Load cameras of site error";
        });
    }break;

    case Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        LayoutStruct layoutStruct = attachment->value<LayoutStruct>();
        int pageNumber = layoutStruct.selectedPage;
        int layoutNumber = layoutStruct.numberOfCameras;
        this->abstraction()->loadCamerasOfSiteWithLayoutPage(pageNumber, layoutNumber, [this]{
            cVideoWall->newAction(Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS, Q_NULLPTR);
        }, [] {
            qDebug() << "Load cameras of site error";
        });
    }break;

    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID:{
        LayoutStruct layoutStruct = attachment->value<LayoutStruct>();
        int pageNumber = layoutStruct.selectedPage;
        int layoutNumber = layoutStruct.numberOfCameras;
        this->abstraction()->loadAllCamerasOfSite([this]{
            cPlayBack->newAction(Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS, Q_NULLPTR);
        }, [] {
            qDebug() << "Load cameras of site error";
        });
    }break;

    case Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_SITE_ID:{
        LayoutStruct layoutStruct = attachment->value<LayoutStruct>();
        int pageNumber = layoutStruct.selectedPage;
        int layoutNumber = layoutStruct.numberOfCameras;
        this->abstraction()->loadAllCamerasOfSite([this]{
            cVideoWall->newAction(Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS, Q_NULLPTR);
        }, [] {
            qDebug() << "Load cameras of site error";
        });
    }break;

    case Message.SHOW_INDICATOR:
    {
        qDebug() << "SHOW_INDICATOR";
        presentation()->showApp(message);
    }
        break;
    case Message.HIDE_INDICATOR:
    {
        qDebug() << "HIDE_INDICATOR";
        presentation()->showApp(message);
    }
        break;

    case Message.APP_NETWORK_IS_REACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_REACHABLE";
        abstraction()->reloadDataWhenNetworkIsReachable();
        cVideoWall->newAction(message, attachment);
    }
        break;
    case Message.APP_NETWORK_IS_UNREACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_UNREACHABLE";
        cVideoWall->newAction(message, attachment);
    }
        break;

    case Message.SHOW_ABOUT_APP:{
        cAboutApp->newAction(message, attachment);

    }break;

    default:

        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}
