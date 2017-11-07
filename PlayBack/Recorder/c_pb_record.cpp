#include "c_pb_record.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBRecord::C_PBRecord(Control* ctrl, QWidget* zone) : Control(ctrl) {
  this->pres = new P_PBRecord(this, zone);
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
void C_PBRecord::newUserAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
    case Message.APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK:
      getParent()->newAction(message, attachment);
      break;
    case Message.APP_PLAY_BACK_START_RECORD_QUICK: {
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
void C_PBRecord::newSystemAction(int message, QVariant* attachment) {
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
void C_PBRecord::newAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
    case Message.APP_SHOW:
      break;
    case Message.PLAYER_RECORD_NORMAL: {
    } break;
    case Message.PLAYER_RECORD_QUICK: {
    } break;
    case Message.APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS: {
      presentation()->downloadVideoRecordSuccess();
    } break;
    case Message.APP_PLAY_BACK_LINK_RECORD_NOT_FOUND: {
      presentation()->linkRecordNotFound();
    } break;
    case Message.APP_PLAY_BACK_CANCEL_SAVE_VIDEO: {
      presentation()->downloadVideoRecordSuccess();
    } break;
    case Message.APP_PLAY_BACK_UPDATE_PROCESS_BAR: {
      int percent = attachment->value<int>();
      presentation()->updateProcessBar(percent);
    } break;
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
