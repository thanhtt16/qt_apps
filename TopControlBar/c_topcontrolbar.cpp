#include "TopControlBar/c_topcontrolbar.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_TopControlBar::C_TopControlBar(Control* ctrl, QWidget* zone) : Control(ctrl) {
  // update app context
  QVariant* dataStruct = new QVariant();
  getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  // init facets
  this->zone = zone;
  this->pres = new P_TopControlBar(this, zone);
  this->abst = new A_TopControlBar(this);

  // create others controls
  // cXXX = new C_XXX();

  cSiteSelector =
      new C_SiteSelector(this, presentation()->getZone(Message.SITE_SELECTOR));

  cAppSelector =
      new C_AppSelector(this, presentation()->getZone(Message.APP_SELECTOR));

  cProfileSetting = new C_ProfileSetting(
        this, presentation()->getZone(Message.PROFILE_SETTING));
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_TopControlBar::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    //    case Message.SITE_CHANGED:
    //      getParent()->newAction(message, attachment);
    //      break;
    case Message.TOP_BAR_OVER_LAY_CAN_HIDE: {
        getParent()->newAction(message, attachment);
      }
    case Message.TOP_BAR_OVER_LAY_CAN_NOT_HIDE: {
        getParent()->newAction(message, attachment);

      } break;
    case Message.SITE_SELECTOR_SHOW:
      cSiteSelector->newAction(Message.SITE_SELECTOR_SHOW, Q_NULLPTR);
      break;

    case Message.APP_SELECTOR_SHOW:
      cAppSelector->newAction(Message.APP_SELECTOR_SHOW, Q_NULLPTR);
      break;

    case Message.PROFILE_SETTING_SHOW:
      cProfileSetting->newAction(Message.PROFILE_SETTING_SHOW, Q_NULLPTR);
      break;

    case Message.SITE_SELECTOR_HIDE:
      cSiteSelector->newAction(Message.SITE_SELECTOR_HIDE, Q_NULLPTR);
      break;

    case Message.APP_SELECTOR_HIDE:
      cAppSelector->newAction(Message.APP_SELECTOR_HIDE, Q_NULLPTR);
      break;

    case Message.PROFILE_SETTING_HIDE:
      cProfileSetting->newAction(Message.PROFILE_SETTING_HIDE, Q_NULLPTR);
      break;

    case Message.APP_VIDEO_WALL_RIGHT_BAR_SHOW:
      ((C_MainFrame*)this->parent)->newAction(message, attachment);
      break;
    case Message.APP_PLAY_BACK_SHOW:
      ((C_MainFrame*)this->parent)->newAction(message, attachment);
      break;
    case Message.APP_VIDEO_WALL_SHOW:
      ((C_MainFrame*)this->parent)->newAction(message, attachment);
      break;
    case Message.TOP_CONTROL_BAR_GET_SITE_LIST: {
        //      qDebug() << Q_FUNC_INFO << attachment;
        //      SiteChild* childSite = attachment->value<SiteChild*>();
        //      if (childSite != Q_NULLPTR) {
        //        ((A_TopControlBar*)this->abst)->setChildSite(childSite);
        //        ((P_TopControlBar*)this->pres)->setChildSite(childSite);
        //      }

      } break;
    case Message.APP_VIDEO_WALL_SHOW_SITE_TOP:
      ((C_MainFrame*)this->parent)->newAction(message, attachment);
      break;
      //    case Message.EXIT_FULLSCREEN_MODE: {
      //      getParent()->newAction(message, attachment);
      //    } break;

    case Message.GET_SIZE_TOP_CONTROL_BAR: {
        cSiteSelector->newAction(message, attachment);
        cAppSelector->newAction(message, attachment);
        cProfileSetting->newAction(message, attachment);
      } break;

    default:
      qDebug() << "ERROR : General User action in" + getClass() +
                  "non-catched :" + Message.toString(message);
      break;
    }
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_TopControlBar::newSystemAction(int message, QVariant* attachment) {
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
void C_TopControlBar::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_SHOW_SETTINGS:
      getParent()->newAction(message, attachment);
      break;

    case Message.APP_PLAY_BACK_SHOW:
      getParent()->newAction(message, attachment);
      break;

    case Message.APP_VIDEO_WALL_SHOW:
      getParent()->newAction(message, attachment);
      break;

    case Message.LOGOUT:
      getParent()->newAction(message, attachment);
      break;

    case Message.EXIT_APP:
      getParent()->newAction(message, attachment);
      break;

    case Message.SITE_NEW_SELECTED:
      getParent()->newAction(message, attachment);
      break;

    case Message.SITE_CHANGED:
      presentation()->siteChanged();
      break;

    case Message.APP_NEW_SELECTED:
      getParent()->newAction(message, attachment);
      break;

    case Message.SHOW_INDICATOR:{
        this->presentation()->disableSelectedApp();
      }break;
    case Message.HIDE_INDICATOR:{
        this->presentation()->enableSelectedApp();
      }break;

    case Message.APP_CHANGED:
      presentation()->appChanged();
      break;

    case Message.APP_CONTEXT_GET:
      attachment->setValue(appContext);
      break;

    case Message.PROFILE_SETTING_HIDE_ALL: {
        getParent()->newAction(Message.TOP_BAR_OVER_LAY_CAN_HIDE, Q_NULLPTR);
        presentation()->hideElementTop();
      } break;

    case Message.SITE_SELECTOR_HIDE_ALL: {
        getParent()->newAction(Message.TOP_BAR_OVER_LAY_CAN_HIDE, Q_NULLPTR);
        presentation()->hideElementTop();
      } break;

    case Message.APP_SELECTOR_HIDE_ALL: {
        getParent()->newAction(Message.TOP_BAR_OVER_LAY_CAN_HIDE, Q_NULLPTR);
        presentation()->hideElementTop();
      } break;

    case Message.DISPLAY_LIST_APPS: {
        cAppSelector->newAction(message, attachment);
      } break;

    case Message.APP_UPDATE_USER_SITES:{
        cSiteSelector->newAction(message, attachment);
      } break;
    case Message.SHOW_ABOUT_APP:{
        getParent()->newAction(message, attachment);

      }break;
      //  case Message.ENTER_FULLSCREEN_MODE:
      //      presentation()->enterFullscreenMode();
      //    break;

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
