#include "c_socket.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_Socket::C_Socket(Control* ctrl, QWidget* zone) : Control(ctrl) {
    // update app context
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_Socket(this, zone);
    this->abst = new A_Socket(this);
    this->zone = zone;
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_Socket::newUserAction(int message, QVariant* attachment) {
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
void C_Socket::newSystemAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_SOCKET_CONNECTED:{
        getParent()->newAction(message, attachment);
    }
        break;
    case Message.APP_PLAY_BACK_GET_ALL_CLIP_RECORD_SUCCESS:{
        getParent()->newAction(message, attachment);
    }
        break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS:{
        getParent()->newAction(message, attachment);
    }
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
void C_Socket::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_CANCEL_RECORD:{
        this->abstraction()->cancelGetClip();
    }break;
    case Message.APP_SOCKET_TO_CONNECT:{
        this->abstraction()->connectToSocket();
    }
        break;

    case Message.SHOW_INDICATOR:{
        this->getParent()->newAction(message, Q_NULLPTR);
    }

    case Message.HIDE_INDICATOR:{
        this->getParent()->newAction(message, Q_NULLPTR);
    }
        break;

    case Message.APP_PLAY_BACK_GET_CLIP_RECORD:{
        DataGetClip dataGetClip = attachment->value<DataGetClip>();
        this->abstraction()->startGetClip(dataGetClip.cameraId, dataGetClip.nameVideo, dataGetClip.urlVideo);
    }break;

    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}
