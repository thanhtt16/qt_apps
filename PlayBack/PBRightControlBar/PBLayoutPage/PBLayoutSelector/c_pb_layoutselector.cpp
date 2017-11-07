#include "c_pb_layoutselector.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBLayoutSelector::C_PBLayoutSelector(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
  QVariant* dataStruct = new QVariant();
  getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  this->pres = new P_PBLayoutSelector(this, zone);
  this->zone = zone;
}

void C_PBLayoutSelector::show(QVariant* attachment) {
  newUserAction(Message.SHOW, attachment);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBLayoutSelector::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_PLAY_BACK_ZONE_LAYOUT_SELECTED: {
      getParent()->newAction(message, attachment);
    } break;
    case Message.APP_PLAY_BACK_PAGE_DEFAULT_SET: {
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
void C_PBLayoutSelector::newSystemAction(int message, QVariant* attachment) {
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
void C_PBLayoutSelector::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET:
      presentation()->setDefaultLayout();
      break;

    case Message.APP_CONTEXT_GET:
      attachment->setValue(this->appContext);
      break;

    case Message.SITE_CHANGED: {
      presentation()->setDefaultLayout();
    } break;

    case Message.APP_PLAY_BACK_PlAYER_SHOW_FULL_SCREEN: {
      presentation()->disabledLayoutButton();
    } break;

    case Message.APP_PLAY_BACK_PlAYER_HIDE_FULL_SCREEN: {
      presentation()->enabledLayoutButton();
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
