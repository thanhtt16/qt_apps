#include "c_siteselector.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_SiteSelector::C_SiteSelector(Control* ctrl, QWidget* zone) : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_SiteSelector(this, zone);
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
void C_SiteSelector::newUserAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.SITE_NEW_SELECTED:
        getParent()->newAction(message, attachment);
        break;
    case Message.SITE_SELECTOR_HIDE_ALL:
        getParent()->newAction(message, attachment);
        break;
    default:
        qDebug() << "ERROR : General User action in" + getClass() +
                    "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_SiteSelector::newSystemAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case 1:
        break;

    default:
        qDebug() << "ERROR : General System action in" + getClass() +
                    "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_SiteSelector::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.SITE_SELECTOR_SHOW:
        presentation()->show();
        break;
    case Message.SITE_SELECTOR_HIDE:
        presentation()->hide();
        break;

    case Message.GET_SIZE_TOP_CONTROL_BAR: {
        presentation()->sizeTopControlBar(attachment);
    } break;

    case Message.APP_UPDATE_USER_SITES: {
        qDebug() << "APP_UPDATE_USER_SITES";
        this->presentation()->displaySites();
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in" + getClass() +
                    "non-catched :" + Message.toString(message);
    }
}
