#include "VideoWall/c_videowall.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_VideoWall::C_VideoWall(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    // init facets
    this->abst = new A_VideoWall(this);
    this->pres = new P_VideoWall(this, zone);
    this->zone = zone;

    cWorkspace = new C_VWWorkSpace(this, presentation()->getZone(1));
    cLayoutPage = new C_VWLayoutPage(this, presentation()->getZone(2));
    qDebug() << "C_VideoWall end";
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VideoWall::newUserAction(int message, QVariant* attachment) {
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
void C_VideoWall::newSystemAction(int message, QVariant* attachment) {
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
void C_VideoWall::newAction(int message, QVariant* attachment) {
    switch (message) {

    case Message.UPDATE_CDN_TYPE_SELECTED:{
        cWorkspace->newAction(message,attachment);
    }break;

    case Message.APP_NETWORK_IS_REACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_REACHABLE";
        cWorkspace->newAction(message, attachment);
    }
        break;
    case Message.APP_NETWORK_IS_UNREACHABLE:
    {
        qDebug() << QDateTime::currentDateTime() << "APP_NETWORK_IS_UNREACHABLE";
        cWorkspace->newAction(message, attachment);
    }
        break;

    case Message.APP_SHOW_SETTINGS: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_CHANGED: {
        QString userAppName = attachment->value<QString>();
        if (userAppName != this->appName) {
            cWorkspace->newAction(message, attachment);
        }
    } break;

    case Message.SITE_CHANGED: {
        cLayoutPage->newAction(message, attachment);
    } break;

    case Message.APP_SHOW: {
        this->appContext->setWorkingApp("Video Wall");
        ((P_VideoWall*)this->pres)->show(this->zone);
        cWorkspace->newAction(Message.APP_SHOW, attachment);
    } break;

    case Message.APP_CONTEXT_GET:
        attachment->setValue(this->appContext);
        break;

    case Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED:
        cWorkspace->newAction(message, attachment);
        break;

    case Message.APP_VIDEO_WALL_SHOW_SITE_TOP:
        cWorkspace->newAction(message, attachment);
        break;

    case Message.ENTER_FULLSCREEN_MODE: {
        presentation()->enterFullscreenMode();
        cWorkspace->newAction(message, attachment);
    } break;

    case Message.EXIT_FULLSCREEN_MODE: {
        presentation()->exitFullscreenMode();
        cWorkspace->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_LAYOUT_DEFAULT_SET: {
        cLayoutPage->newAction(message, attachment);
    } break;
    case Message.PAGE_TRANSITION_BEGIN: {
        cLayoutPage->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT: {
        cWorkspace->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_LAYOUT_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }break;

    case Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE:{
        this->getParent()->newAction(message,attachment);
    }break;

    case Message.APP_VIDEO_WALL_LOAD_CAMERAS_OF_SITE_WITH_SITE_ID_SUCCESS:{
        cWorkspace->newAction(message, attachment);
    }break;

    case Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_SITE_ID:{
        this->getParent()->newAction(message,attachment);
    }break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" << Message.toString(message);
    }
}
