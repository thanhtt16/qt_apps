#include "c_cam9player.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_Cam9Player::C_Cam9Player(Control *ctrl, QWidget *zone) : Control(ctrl) {
    QVariant *dataStruct = new QVariant();
    getPbWorkSpace()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext *>();
    this->zone = zone;
    this->pres = new P_Cam9Player(this, this->zone);
    modePlayerCurrent = this->presentation()->getModeOneVideoVOD();
//    connect(this, &C_Cam9Player::setPlayerDisplayName, this->presentation(), &P_Cam9Player::setPlayerDisplayName);
    cPlayerOverlay = new C_Cam9PlayerOverlay(this, presentation()->getZone(1));
}

void C_Cam9Player::show(QVariant *attachment) {
    newUserAction(Message.SHOW, attachment);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Cam9Player::newUserAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        C_PBWorkSpace* workSpace = (C_PBWorkSpace*)(this->parent);
        if (workSpace != Q_NULLPTR) {
            workSpace->newAction(message, attachment);
        }

    } break;
    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        C_PBWorkSpace* workSpace = (C_PBWorkSpace*)(this->parent);
        if (workSpace != Q_NULLPTR) {
            workSpace->newAction(message, attachment);
        }
    } break;

    default:
        qDebug() << "ERROR : General User action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Cam9Player::newSystemAction(int message, QVariant *attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case 1:
        break;

    default:
        qDebug() << "ERROR : General System action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Cam9Player::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_UPDATE_TIMESTAMP_FOR_PLAYER:{
        TimeRange timerangeCurrent = attachment->value<TimeRange>();
        presentation()->updateTimestampCurrent(timerangeCurrent);
    }break;

    case Message.APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION: {
        int positionNew = attachment->value<int>();
        presentation()->seekToNewPostion(positionNew);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        getPbWorkSpace()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        getPbWorkSpace()->newAction(message, attachment);

    } break;

    case Message.PLAYER_PLAY: {
        presentation()->cam9Play();
    } break;

    case Message.PLAYER_PLAY_VOD_RTC: {
        QString url = attachment->value<QString>();
        presentation()->playVOD(url, 2.0);
    } break;

    case Message.PLAYER_LOADING: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAYING: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_PAUSED: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_STOPED: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_NEW_VOD_SOURCE_SET: {
        VideoVOD *videoVod = attachment->value<VideoVOD *>();
        presentation()->cam9Play(videoVod);
        resetPlayerSpeed();
    } break;

    case Message.PLAYER_NEW_VOD_SOURCE_ONE_VIDEO_SET: {
        VideoVOD *videoVod = attachment->value<VideoVOD *>();
        presentation()->playOneVideoVod(videoVod);
    } break;

    case Message.PLAYER_SOURCE_CLEAR: {
        presentation()->cam9Stop();
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_START_LOADING: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_STOP_LOADING: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_UPDATE_INFO: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

        //            case Message.PLAYER_RELASE: {
        //              cPlayerOverlay->newAction(message, attachment);
        //            } break;

    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION: {
        long timeStampStart = attachment->value<long>();
        presentation()->seekToNewPostion(timeStampStart);
    } break;

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN: {
        if (getPbWorkSpace() != Q_NULLPTR) {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(getDisplayName().toInt());
            getPbWorkSpace()->newAction(message, dataStruct);
        }
    } break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN: {
        if (getPbWorkSpace() != Q_NULLPTR) {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(getDisplayName().toInt());
            getPbWorkSpace()->newAction(message, dataStruct);
        }
    } break;

    case Message.PLAYER_END_SHOW_FULLSCREEN: {
        cPlayerOverlay->newAction(message, Q_NULLPTR);
        presentation()->setSpeed(1.0);
    } break;

    case Message.PLAYER_END_HIDE_FULLSCREEN: {
        cPlayerOverlay->newAction(message, Q_NULLPTR);
        resetPlayerSpeed();
    } break;

    case Message.PLAYER_RECORD_NORMAL: {
        if (getPbWorkSpace()) {
            getPbWorkSpace()->newAction(message, attachment);
        }
    } break;

    case Message.PLAYER_RECORD_QUICK: {
        if (getPbWorkSpace()) {
            getPbWorkSpace()->newAction(message, attachment);
        }
    } break;

    case Message.PLAYER_TAKE_SCREENSHOT: {
        presentation()->takeScreenshoot();
    } break;


    case Message.PLAYER_PLAY_VOD_SD: {
        presentation()->playVodSd();
        QVariant *dataStruct = new QVariant();
        long playerId = getDisplayName().toLong();
        dataStruct->setValue<long>(playerId);
        getPbWorkSpace()->newAction(message, dataStruct);
        resetPlayerSpeed();
    } break;

    case Message.PLAYER_PLAY_VOD_HD: {
        presentation()->playVodHd();
        QVariant *dataStruct = new QVariant();
        long playerId = getDisplayName().toLong();
        dataStruct->setValue<long>(playerId);
        getPbWorkSpace()->newAction(message, dataStruct);
        resetPlayerSpeed();
    } break;

    case Message.PLAYER_PLAY_SD: {
        cPlayerOverlay->newAction(message, attachment);
        resetPlayerSpeed();
    } break;

    case Message.PLAYER_PLAY_HD: {
        cPlayerOverlay->newAction(message, attachment);
        resetPlayerSpeed();
    } break;

    case Message.APP_PLAY_BACK_RESET_PLAYER_SPEED:
    case Message.APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO: {
        double speed = attachment->value<double>();
        presentation()->setSpeed(speed);
    } break;

    case Message.APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL: {
        cPlayerOverlay->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL: {
        cPlayerOverlay->newAction(message, attachment);

    } break;

    case Message.PLAYER_PLAY_HD_CLICK: {
        // load playlist
        getPbWorkSpace()->newAction(message, attachment);
        // set speed to 1.0
        resetPlayerSpeed();

        presentation()->playHD();
    } break;

    case Message.PLAYER_PLAY_SD_CLICK: {
        // load playlist
        getPbWorkSpace()->newAction(message, attachment);
        // set speed to 1.0
        resetPlayerSpeed();
        presentation()->playSD();
    } break;

    case Message.PLAYER_EXIT_POP_OUT_MODE: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.WHEEL_EVENT_ZOOM_VIDEO: {
        presentation()->wheelEventZoomVideo(attachment);
    } break;

    case Message.PLAYER_PLAY_BACK_UPDATE_MODE:{
        QString modeCurrent = attachment->value<QString>();
        modePlayerCurrent = modeCurrent;
        presentation()->setModeOneVideoVOD(modeCurrent);
    }break;

    case Message.EXIT_FULLSCREEN_MODE:{
        presentation()->updateSizeWhenExitFullScreenMode();
    }break;

    case Message.ENTER_FULLSCREEN_MODE:{
        presentation()->updateSizeWhenEnterFullScreenMode();
    }break;


    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

QString C_Cam9Player::getModePlayerCurrent() const
{
    return modePlayerCurrent;
}

// chỉ dùng cho app playback

void C_Cam9Player::resetPlayerSpeed() {
    presentation()->setSpeed(1.0);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(1.0);
    getPbWorkSpace()->newAction(Message.APP_PLAY_BACK_RESET_PLAYER_SPEED,
                                dataStruct);
}
