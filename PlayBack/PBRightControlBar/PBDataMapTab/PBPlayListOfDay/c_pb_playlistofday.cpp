#include "c_pb_playlistofday.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBPlayListOfDay::C_PBPlayListOfDay(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_PBPlayListOfDay(this, zone);
    this->zone = zone;

}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBPlayListOfDay::newUserAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR: {
        getParent()->newAction(message, Q_NULLPTR);
    } break;

    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION:
        getParent()->newAction(message, attachment);
        break;
    case Message.APP_PLAY_BACK_SELECTED_TIME_SLOT: {
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
void C_PBPlayListOfDay::newSystemAction(int message, QVariant* attachment) {
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
void C_PBPlayListOfDay::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS:{
        QList<int> mapStateHours = attachment->value<QList<int>>();
        this->presentation()->updateStateOfHours(mapStateHours);
    }break;

    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED: {
        QDateTime newDate = attachment->value<QDateTime>();
        presentation()->setSelectedDate(newDate);
    } break;
    case Message.APP_PLAY_BACK_UPDATE_TIME_PLAYLISTOFDAY: {
        presentation()->updateTimeCurrent();
    } break;
    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to create a new pac agent exactly equals to the agent attached
  *to
  *this control.
     * @return the control of the created clone.
     **/
// Control Control::getClone(){
//    try{
//        Presentation 	P = Q_NULLPTR;
//        Abstraction 	A = Q_NULLPTR;
//        Control 		C = (Control)clone();

//        if (pres != null)
//            P = (Presentation)(pres.getClone());
//        if (abst != null)
//            A = (Abstraction)(abst.getClone());

//        C.changeFacets(P, A);
//        return C;
//    } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//    control.");}
//    return null;
//}
