#include "c_appselector.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_AppSelector::C_AppSelector(Control* ctrl, QWidget* zone) : Control(ctrl) {
  QVariant* dataStruct = new QVariant();
  getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  this->pres = new P_AppSelector(this, zone);
  this->zone = zone;

  // create others controls
  // cXXX = new C_XXX();

  // presentation()->loadDefaultWorkingApp();
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_AppSelector::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_NEW_SELECTED: {
      UserApp newWorkingApp = attachment->value<UserApp>();
      this->appContext->setWorkingApp(newWorkingApp);
      getParent()->newAction(message, attachment);
      switch (newWorkingApp.appCode) {
        case 1:
          getParent()->newAction(Message.APP_VIDEO_WALL_SHOW, Q_NULLPTR);
          break;
        case 2:
          getParent()->newAction(Message.APP_PLAY_BACK_SHOW, Q_NULLPTR);
          break;
        default:
          break;
      }
    } break;
    case Message.APP_SELECTOR_HIDE_ALL:
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
void C_AppSelector::newSystemAction(int message, QVariant* attachment) {
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
void C_AppSelector::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_SELECTOR_SHOW:
      presentation()->show();
      break;

    case Message.APP_SELECTOR_HIDE:
      presentation()->hide();
      break;
    case Message.DISPLAY_LIST_APPS: {
      presentation()->displayApps();
    } break;

    case Message.GET_SIZE_TOP_CONTROL_BAR: {
      presentation()->sizeTopControlBar(attachment);
    } break;
    default:
      qDebug() << "ERROR : General Internal pac action in" + getClass() +
                      "non-catched :" + Message.toString(message);
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
