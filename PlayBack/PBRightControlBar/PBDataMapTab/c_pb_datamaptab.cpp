#include "c_pb_datamaptab.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBDataMapTab::C_PBDataMapTab(Control* ctrl, QWidget* zone) : Control(ctrl) {
  this->pres = new P_PBDataMapTab(this, zone);
  this->zone = zone;

  cCalendar = new C_PBCalendar(
      this,
      presentation()->getZone(Message.APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR));

  cPlayListOfDay = new C_PBPlayListOfDay(
      this,
      presentation()->getZone(
          Message.APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY));
}

void C_PBDataMapTab::show(QVariant* attachment) {
  newUserAction(Message.SHOW, attachment);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBDataMapTab::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.SHOW:
      presentation()->show(attachment);
      break;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE:
      cPlayListOfDay->newUserAction(
          Message.APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE, attachment);
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
void C_PBDataMapTab::newSystemAction(int message, QVariant* attachment) {
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
void C_PBDataMapTab::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION:
      //      getParent()->newAction(message, attachment);
      break;
    case Message.APP_SHOW:
      break;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE:
      cPlayListOfDay->newAction(
          Message.APP_PLAY_BACK_ZONE_DATA_MAP_CHANGE_SELECT_DATE, attachment);
      break;
    case Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR:
      cCalendar->newAction(Message.APP_PLAY_BACK_UPDATE_MAX_DATE_CALENDAR,
                           Q_NULLPTR);
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
