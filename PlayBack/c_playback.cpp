#include "PlayBack/c_playback.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PlayBack::C_PlayBack(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    // init facets
    this->abst = new A_PlayBack(this);
    this->pres = new P_PlayBack(this, zone);
    this->zone = zone;

    // init workspace
    cWorkspace = new C_PBWorkSpace(this, presentation()->getZone(1));
    // create others controls
    // cRightControlBar = new C_PBRightControlBar(this,
    // presentation()->getZone(2));
    cLayoutPage = new C_PBLayoutPage(this, presentation()->getZone(2));
//    cLayoutPage->newAction(Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET, Q_NULLPTR);

    qDebug() << "C_PlayBack Success";
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PlayBack::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_CONTEXT_GET:
        break;

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
void C_PlayBack::newSystemAction(int message, QVariant* attachment) {
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
void C_PlayBack::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.UPDATE_CDN_TYPE_SELECTED:{
        cWorkspace->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY:{
        cLayoutPage->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS:{
        cLayoutPage->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR:{
        cLayoutPage->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_CANCEL_RECORD:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_CHANGED: {
        cLayoutPage->newAction(message,attachment);
        QString userAppName = attachment->value<QString>();
        if (userAppName != this->appName)
            cWorkspace->newAction(message, attachment);
    } break;

    case Message.APP_SHOW_SETTINGS: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION:{
        cWorkspace->newAction(message, attachment);
    }
        break;

    case Message.SITE_CHANGED: {
        cLayoutPage->newAction(message, attachment);
    } break;

    case Message.APP_SHOW:
        cLayoutPage->newAction(message,attachment);
        this->appContext->setWorkingApp("Playback");
        ((P_PlayBack*)this->pres)->show(this->zone);
        cWorkspace->newAction(Message.APP_SHOW, attachment);
        break;

    case Message.APP_CONTEXT_GET:
        attachment->setValue(this->appContext);
        break;

    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED:
        cWorkspace->newAction(message, attachment);
        break;

    case Message.APP_PLAY_BACK_SHOW_SITE_TOP:
        cWorkspace->newAction(message, attachment);
        break;

    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED:
        cWorkspace->newAction(message, attachment);
        break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
        //      cLayoutPage->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
        //      cLayoutPage->newAction(message, attachment);
    } break;

    case Message.ENTER_FULLSCREEN_MODE:
        presentation()->enterFullscreenMode();
        cWorkspace->newAction(message, attachment);
        break;

    case Message.EXIT_FULLSCREEN_MODE: {
        presentation()->exitFullscreenMode();
        cWorkspace->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR: {
        cLayoutPage->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY: {
        cLayoutPage->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET: {
        cLayoutPage->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_UPDATE_PAGE_DEFAULT: {
        cWorkspace->newAction(message, attachment);
    } break;

    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD:{
        getParent()->newAction(message, attachment);
    }break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }
        break;

    case Message.APP_PLAY_BACK_GET_ALL_CLIP_RECORD_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }
        break;
    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}
