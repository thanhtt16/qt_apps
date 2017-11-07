#include "c_pb_calendar.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBCalendar::C_PBCalendar(Control* ctrl, QWidget* zone) : Control(ctrl) {
    QVariant* dataStruct = new QVariant();
    getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
    this->appContext = dataStruct->value<AppContext*>();

    this->pres = new P_PBCalendar(this, zone);
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
void C_PBCalendar::newUserAction(int message, QVariant* attachment) {
    switch (message) {

    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED: {
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
void C_PBCalendar::newSystemAction(int message, QVariant* attachment) {
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
void C_PBCalendar::newAction(int message, QVariant* attachment) {
    Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_PLAY_BACK_CALENDAR_MAX_HISTORYDAY:{
//        presentation()->updateMaxDayCalendar();
    }break;

    case Message.SITE_CHANGED:
    case Message.APP_SHOW:
    case Message.APP_CHANGED: {
        QList<QHash<QDate, int>> listStateOfDates;
        this->presentation()->updateStateOfDate(listStateOfDates);
    }break;

    case Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR:{
        QList<QHash<QDate, int>> listStateOfDates = attachment->value<QList<QHash<QDate, int>>>();
        this->presentation()->updateStateOfDate(listStateOfDates);
    }break;
    case Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR:
//        presentation()->updateMaxDayCalendar();
        break;
    default:
        qDebug() << "ERROR : General Internal pac action in" << getClass()
                 << "non-catched :" + Message.toString(message);
    }
}

/**
     * Method to create a new pac agent exactly equals to the agent attached to
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
