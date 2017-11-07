#include "c_cam9rtcplayer.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_Cam9RTCPlayer::C_Cam9RTCPlayer(Control *ctrl, QWidget *zone) : Control(ctrl) {
    QVariant *dataStruct = new QVariant();
    getPbWorkSpace()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext *>();
    this->zone = zone;
    this->pres = new P_Cam9RTCPlayer(this, this->zone);

    rtcPlayer = new RTCPlayer(presentation()->getZone(3));
    connect(this, &C_Cam9RTCPlayer::setPlayerDisplayName, rtcPlayer, &RTCPlayer::onSetPlayerDisplayName);
    //    rtcPlayer->setPlayerName(this->getDisplayName());

    connect(rtcPlayer, &RTCPlayer::showLoadingSign, presentation(), &P_Cam9RTCPlayer::onShowLoadingSign);
    connect(rtcPlayer, &RTCPlayer::showPlayerLayer, presentation(), &P_Cam9RTCPlayer::onShowPlayerLayer);
    connect(rtcPlayer, &RTCPlayer::showNoCameraLayer, presentation(), &P_Cam9RTCPlayer::onShowWallLayer);

    //playbackPlayer = new PlaybackPlayer(this->zone);
    //mainLayout->addWidget(playbackPlayer);

    //    connect(this, &P_Cam9RTCPlayer::setPlayerDisplayName, playbackPlayer, &PlaybackPlayer::onSetPlayerDisplayName);
    //    connect(playbackPlayer, &PlaybackPlayer::showLoadingSign, this, &P_Cam9RTCPlayer::onShowLoadingSign);
    //    connect(playbackPlayer, &PlaybackPlayer::showPlayerLayer, this, &P_Cam9RTCPlayer::onShowPlayerLayer);
    //    connect(playbackPlayer, &PlaybackPlayer::showWallLayer, this, &P_Cam9RTCPlayer::onShowWallLayer);

    cPlayerOverlay = new C_Cam9RTCPlayerOverlay(this, presentation()->getZone(1));
    this->presentation()->onShowWallLayer();
}

void C_Cam9RTCPlayer::show(QVariant *attachment) {
    newUserAction(Message.SHOW, attachment);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Cam9RTCPlayer::newUserAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.PLAYER_LOADING: {
        cPlayerOverlay->newAction(message, attachment);
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
void C_Cam9RTCPlayer::newSystemAction(int message, QVariant *attachment) {
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
void C_Cam9RTCPlayer::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.APP_NETWORK_IS_REACHABLE:
    case Message.APP_NETWORK_IS_UNREACHABLE: {
        this->rtcPlayer->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAYING:{
        cPlayerOverlay->newAction(message, attachment);
    }break;

    case Message.PLAYER_SOURCE_CLEAR: {
        this->rtcPlayer->newAction(message, attachment);
        cPlayerOverlay->newAction(message, attachment);
        presentation()->clearSource();
    } break;

    case Message.PLAYER_NEW_LIVE_SOURCE_SET: {
        cPlayerOverlay->newAction(message, attachment);
        this->rtcPlayer->newAction(message, attachment);
        presentation()->showLoadingZone();
    } break;

    case Message.PLAYER_PLAY_LIVE_HD:
    case Message.PLAYER_PLAY_LIVE_SD: {
        //cPlayerOverlay->newAction(message, attachment);
        this->rtcPlayer->newAction(message, attachment);
    } break;

    case Message.PLAYER_PAUSED: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_STOPED: {
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

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN: {
        if (getVwWorkSpace() != Q_NULLPTR) {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(getDisplayName().toInt());
            getVwWorkSpace()->newAction(message, dataStruct);
        }
    } break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN: {
        if (getVwWorkSpace() != Q_NULLPTR) {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(getDisplayName().toInt());
            getVwWorkSpace()->newAction(message, dataStruct);
        }
    } break;

    case Message.PLAYER_END_SHOW_FULLSCREEN: {
        cPlayerOverlay->newAction(message, Q_NULLPTR);
    } break;

    case Message.PLAYER_END_HIDE_FULLSCREEN: {
        cPlayerOverlay->newAction(message, Q_NULLPTR);
    } break;

    case Message.PLAYER_TAKE_SCREENSHOT: {
        rtcPlayer->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_SD: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_HD: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.PLAYER_EXIT_POP_OUT_MODE: {
        cPlayerOverlay->newAction(message, attachment);
    } break;

    case Message.WHEEL_EVENT_ZOOM_VIDEO: {
        rtcPlayer->newAction(message, attachment);
    } break;

    case Message.EVENT_MOVE_ZOOM_VIDEO: {
        rtcPlayer->newAction(message, attachment);
    } break;

    case Message.ENTER_FULLSCREEN_MODE:{
        presentation()->updateSizeWhenEnterFullScreenMode();
    }break;

    case Message.EXIT_FULLSCREEN_MODE:{
        presentation()->updateSizeWhenExitFullScreenMode();
    }break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
