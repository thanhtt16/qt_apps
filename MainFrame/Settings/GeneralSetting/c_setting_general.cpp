#include "c_setting_general.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_SettingGeneral::C_SettingGeneral(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();
    this->pres = new P_SettingGeneral(this, zone);
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
void C_SettingGeneral::newUserAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case 1:
        break;
    case Message.UPDATE_CDN_TYPE_SELECTED:{
        getParent()->newAction(message,attachment);
    }break;

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
void C_SettingGeneral::newSystemAction(int message, QVariant* attachment) {
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
void C_SettingGeneral::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_SHOW:
        break;
    case Message.SITE_CHANGED: {
        Site* site = attachment->value<Site*>();
        presenation()->updateWorkingSite(site);
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
