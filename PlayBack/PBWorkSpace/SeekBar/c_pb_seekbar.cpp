#include "c_pb_seekbar.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBSeekBar::C_PBSeekBar(Control* ctrl, QWidget* zone) : Control(ctrl) {
    this->pres = new P_PBSeekBar(this, zone);
    this->zone = zone;

    // create others controls
    // cXXX = new C_XXX();
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBSeekBar::newUserAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION: {
        getParent()->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO:
        getParent()->newAction(message, attachment);
        break;
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
void C_PBSeekBar::newSystemAction(int message, QVariant* attachment) {
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
void C_PBSeekBar::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_CHANGED:{
        this->presentation()->stopWhenChangeApp();
    }
        break;

    case Message.APP_SHOW:{
        presentation()->resetValueWhenAppShow();
    }break;

    case Message.PLAYER_BEGIN_SHOW_FULLSCREEN:{
        presentation()->enableSpeedButton();
    }break;

    case Message.PLAYER_BEGIN_HIDE_FULLSCREEN:{
        presentation()->disableSpeedButton();
    }break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        presentation()->enableSpeedButton();
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        presentation()->disableSpeedButton();
    } break;

    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION: {
        long timeStampStart = attachment->value<long>();
        presentation()->seekToNewPostion(timeStampStart);
    } break;

    case Message.APP_PLAY_BACK_RESET_PLAYER_SPEED: {
        presentation()->resetPlayerSpeed();
    } break;

    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED: {
        presentation()->resetValueSeekBar();
    } break;

    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED: {
        LayoutStruct layoutStruct = attachment->value<LayoutStruct>();
        presentation()->disableSpeedButton(layoutStruct);
    } break;
    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
