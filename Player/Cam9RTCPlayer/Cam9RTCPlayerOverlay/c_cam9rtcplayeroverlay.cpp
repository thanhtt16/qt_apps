#include "c_cam9rtcplayeroverlay.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_Cam9RTCPlayerOverlay::C_Cam9RTCPlayerOverlay(Control *ctrl, QWidget *zone)
    : Control(ctrl) {
    this->zone = zone;
    this->pres = new P_Cam9RTCPlayerOverlay(this, this->zone);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Cam9RTCPlayerOverlay::newUserAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.PLAYER_RECORD_NORMAL: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_RECORD_QUICK: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_TAKE_SCREENSHOT: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_LIVE_HD:
    case Message.PLAYER_PLAY_LIVE_SD: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_VOD_SD: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_VOD_HD: {
        this->getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.WHEEL_EVENT_ZOOM_VIDEO: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.EVENT_MOVE_ZOOM_VIDEO: {
        getParent()->newAction(message, attachment);
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
void C_Cam9RTCPlayerOverlay::newSystemAction(int message, QVariant *attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.PLAYER_PLAY_HD_CLICK: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_PLAY_SD_CLICK: {
        getParent()->newAction(message, attachment);
    } break;

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
void C_Cam9RTCPlayerOverlay::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.PLAYER_START_LOADING: {
        presentation()->showLoader();
    } break;

    case Message.PLAYER_STOP_LOADING: {
        presentation()->hideLoader();
    } break;

    case Message.PLAYER_LOADING: {
        presentation()->playerLoading();
    } break;

    case Message.PLAYER_PLAYING: {
        presentation()->playerPlaying();
    } break;

    case Message.PLAYER_STOPED: {
        presentation()->playerStopped();
    } break;

    case Message.PLAYER_SOURCE_CLEAR: {
//        presentation()->hideAll();
        presentation()->clearStateLive();
    } break;

    case Message.PLAYER_NEW_LIVE_SOURCE_SET: {
        CamItem *camItem = attachment->value<CamItem *>();
        presentation()->updateInfo(camItem);
     } break;

    case Message.PLAYER_UPDATE_INFO: {
        CamItem *camItem = attachment->value<CamItem *>();
        presentation()->updateInfo(camItem);
    } break;

    case Message.PLAYER_END_SHOW_FULLSCREEN: {
        //      presentation()->onShownFullscreen();
    } break;

    case Message.PLAYER_END_HIDE_FULLSCREEN: {
        //      presentation()->onHiddenFullscreen();
    } break;

    case Message.PLAYER_PLAY_SD: {
        presentation()->displaySelectedSd();
    } break;

    case Message.PLAYER_PLAY_HD: {
        presentation()->displaySelectedHd();
    } break;

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.PLAYER_EXIT_POP_OUT_MODE: {
        presentation()->exitPopOutMode();
    } break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL: {
        presentation()->startAnimationNormalButton();
    } break;

    case Message.APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL: {
        presentation()->stopAnimationNormalButton();

    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
