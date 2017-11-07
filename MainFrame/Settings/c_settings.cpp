#include "c_settings.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
AppContext* C_Settings::getAppContext() const { return appContext; }

void C_Settings::setAppContext(AppContext* value) { appContext = value; }

C_Settings::C_Settings(Control* ctrl, QWidget* zone) : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_Settings(this, zone);
    this->abst = new A_Settings(this);
    this->zone = zone;

    this->cSettingGeneral =
            new C_SettingGeneral(this, presentation()->getZone(0));
    this->cSettingVideoWall =
            new C_SettingVideoWall(this, presentation()->getZone(1));
    this->cSettingPlayBack =
            new C_SettingPlayBack(this, presentation()->getZone(2));
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Settings::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case 1:
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
void C_Settings::newSystemAction(int message, QVariant* attachment) {
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
void C_Settings::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.UPDATE_CDN_TYPE_SELECTED:{
        getParent()->newAction(message,attachment);
    }break;

    case Message.APP_SHOW_SETTINGS: {
        int index = attachment->value<int>();
        index = abs(index) % 3;
        this->presentation()->showSettings(index);
    } break;

    case Message.APP_HIDE_SETTINGS: {
        this->presentation()->hideSettings();
    } break;

    case Message.APP_UPDATE_INDEX_SETTINGS: {
        int index = attachment->value<int>();
        index = abs(index) % 3;
        this->presentation()->selectTab(index);
    } break;

    case Message.PAGE_TRANSITION_START_ON_CLICK: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.PAGE_TRANSITION_START_AUTOMATICALLY: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.PAGE_TRANSITION_DELAY: {
        getParent()->newAction(message, attachment);
    } break;
    case Message.SITE_CHANGED: {
        cSettingGeneral->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_SET_PATH_SAVE_MEDIA: {
        getParent()->newAction(message, attachment);
    } break;

    case Message.APP_CONTEXT_GET:
        attachment->setValue(appContext);
        break;
    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
