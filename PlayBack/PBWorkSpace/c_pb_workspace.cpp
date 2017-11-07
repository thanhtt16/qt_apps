#include "c_pb_workspace.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBWorkSpace::C_PBWorkSpace(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    // init facets
    this->abst = new A_PBWorkSpace(this);
    this->pres = new P_PBWorkSpace(this, zone);
    this->zone = zone;

    // create player controls
    for (int index = 0; index < Message.APP_PLAY_BACK_MAX_NUMBER_OF_PLAYERS;
         ++index) {
        QWidget* newZone = presentation()->getZone(index);
        Cam9PlayerPool::instance().getPlayer(this, newZone, appName,
                                             QString::number(index));
    }

    cSeekBar = new C_PBSeekBar(this, presentation()->getZone(-1));
    cRecoder = new C_PBRecord(this, presentation()->getZone(-2));
}
/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBWorkSpace::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS: {
        cRecoder->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_LINK_RECORD_NOT_FOUND: {
        cRecoder->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_UPDATE_PROCESS_BAR: {
        cRecoder->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_CANCEL_SAVE_VIDEO: {
        cRecoder->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        getParent()->newAction(message,attachment);
    }break;
    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD:{
        getParent()->newAction(message, attachment);
    }break;
    default:
        qDebug() << "ERROR : General User action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBWorkSpace::newSystemAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_CONTEXT_GET:
        attachment->setValue(this->appContext);
        break;

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
void C_PBWorkSpace::newAction(int message, QVariant* attachment) {
    switch (message) {

    case Message.UPDATE_CDN_TYPE_SELECTED:{
        CamItemType type = attachment->value<CamItemType>();
        this->presentation()->updateNetworkTypeSelected(type);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_SEEK_BAR:{
        cSeekBar->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_CANCEL_RECORD:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_CHANGED:{
        presentation()->stopPlaying();
        cSeekBar->newAction(message, attachment);
    }
        break;

    case Message.APP_SHOW:{
        presentation()->refreshAppShow();
        cSeekBar->newAction(message, attachment);
    }
        break;
    case Message.APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION: {
        presentation()->seekbarToNewPosition(attachment);
    } break;
    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION: {
        // cSeekControlBar->newAction(message, attachment);
        long timeStampStart = attachment->value<long>();
        QString timeStampString = QString::number(timeStampStart);

        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("playback");
        settings.setValue("selected_time_slot", timeStampString);
        presentation()->seekToNewPostion(timeStampStart);
        cSeekBar->newAction(message, attachment);

    } break;

    case Message.APP_CONTEXT_GET: {
        attachment->setValue(this->appContext);
    } break;

    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED: {
        LayoutStruct layout = attachment->value<LayoutStruct>();

        QSettings settings;
        settings.beginGroup(QString::number(this->appContext->getWorkingUser()->getUserId()));
        settings.beginGroup("playback");
        settings.setValue("selected_layout", layout.code);
        settings.setValue("selected_page", layout.selectedPage);
        cSeekBar->newAction(message, attachment);
        presentation()->refresh(layout);
    } break;

    case Message.APP_PLAY_BACK_SHOW_SITE_TOP: {
        siteSelector->newAction(message, attachment);
    } break;

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN: {
        int index = attachment->value<int>();
        presentation()->showFullCellAt(index);
        cSeekBar->newAction(message, attachment);

    } break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN: {
        int index = attachment->value<int>();
        presentation()->hideFullCellAt(index);
        cSeekBar->newAction(message, attachment);

    } break;

    case Message.PLAYER_END_SHOW_FULLSCREEN: {
        C_Cam9Player* player = attachment->value<C_Cam9Player*>();
        if (player != Q_NULLPTR) {
            player->newAction(message, Q_NULLPTR);
        }
    } break;

    case Message.PLAYER_END_HIDE_FULLSCREEN: {
        C_Cam9Player* player = attachment->value<C_Cam9Player*>();
        if (player != Q_NULLPTR) {
            player->newAction(message, Q_NULLPTR);
        }
    } break;
    case Message.APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT: {
        TimeRange timeStampCurrent = attachment->value<TimeRange>();
        presentation()->setTimeRangeCurrent(timeStampCurrent);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        getParent()->newAction(message, attachment);
        cSeekBar->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        getParent()->newAction(message, attachment);
        presentation()->hideRecordDetail();
        cSeekBar->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD: {
        TimeRange timeRange = attachment->value<TimeRange>();
        presentation()->playListVideoVodWithTimeStamp(timeRange);
    } break;

    case Message.APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD: {
        TimeRange timeRange = attachment->value<TimeRange>();
        presentation()->playOneVideoVodWithTimeStamp(timeRange);
    } break;

    case Message.PLAYER_PLAY_VOD_SD: {
        long playerId = attachment->value<long>();
        QString modeVideo = "SD";
        presentation()->fetchOneVideoVODSwitchMode(playerId);
    } break;

    case Message.PLAYER_PLAY_VOD_HD: {
        QString modeVideo = "HD";
        int playerId = attachment->value<int>();
        presentation()->fetchOneVideoVODSwitchMode(playerId);
    } break;

    case Message.APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO: {
        presentation()->changeSpeedAllVideo(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_RESET_PLAYER_SPEED: {
        presentation()->changeSpeedAllVideo(message, attachment);
        cSeekBar->newAction(message, attachment);
    } break;

    case Message.PLAYER_RECORD_NORMAL: {
        presentation()->hideRecordDetail();
        presentation()->recordVideoDefault();
        cRecoder->newAction(message, attachment);
    } break;

    case Message.PLAYER_RECORD_QUICK: {
        presentation()->showRecordDetail();
    } break;
    case Message.APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK: {
        presentation()->hideRecordDetail();
    } break;
    case Message.APP_PLAY_BACK_START_RECORD_QUICK: {
        TimeStampRecord timeRecord = attachment->value<TimeStampRecord>();
        presentation()->recordVideoQuick(timeRecord);
    } break;
    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED: {
        QDate dateSelectedCurrent = attachment->value<QDate>();
        this->presentation()->updateDateSelectedCurrent(dateSelectedCurrent);
        cSeekBar->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_UPDATE_PAGE_DEFAULT: {
        LayoutStruct defaultLayout = attachment->value<LayoutStruct>();
        presentation()->updateDefaultLayout(defaultLayout);
    } break;


    case Message.APP_PLAY_BACK_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS:{
        presentation()->refreshNewLayoutPage();
    }break;


    case Message.APP_PLAY_BACK_GET_ALL_CLIP_RECORD_SUCCESS:{
        QString url_file_mp4 = attachment->value<QString>();
        this->presentation()->startDownLoadVideoRecord(url_file_mp4);
    }
        break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS:{
        QString url_file_mp4 = attachment->value<QString>();
        this->presentation()->startDownLoadVideoRecord(url_file_mp4);
    }
        break;

    case Message.ENTER_FULLSCREEN_MODE :{
        presentation()->resizePlayerWhenEnterFullScreen();
    }break;
    case Message.EXIT_FULLSCREEN_MODE :{
        presentation()->resizePlayerWhenExitFullScreen();
    }break;


    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}
