#include "c_network.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_Network::C_Network(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_Network(this, zone);
    this->abst = new A_Network(this);
    this->zone = zone;
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Network::newUserAction(int message, QVariant* attachment) {
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
void C_Network::newSystemAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_NETWORK_IS_REACHABLE: {
        newAction(message, attachment);
    } break;

    case Message.APP_NETWORK_IS_UNREACHABLE: {
        newAction(message, attachment);
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
void C_Network::newAction(int message, QVariant* attachment) {
    switch (message) {
    case  Message.STOP_TIMER_RECHECK_NETWORK:{
        this->abstraction()->stopTimerRecheck();
    }break;

    case Message.APP_NETWORK_IS_REACHABLE: {
        getParent()->newAction(message, attachment);
        this->presentation()->hideDialog();
    } break;

    case Message.APP_NETWORK_IS_UNREACHABLE: {
        getParent()->newAction(message, attachment);
        this->presentation()->showDialog("Không có kết nối mạng!");
    } break;

    case Message.APP_NETWORK_START_CHECKING: {
        abstraction()->startChecking();
        this->presentation()->showDialog("Đang kiểm tra kết nối mạng...");
    } break;

    case Message.APP_NETWORK_STOP_CHECKING: {
        abstraction()->stopChecking();
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
