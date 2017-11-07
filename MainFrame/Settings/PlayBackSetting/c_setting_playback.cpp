#include "c_setting_playback.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
AppContext* C_SettingPlayBack::getAppContext() const { return appContext; }

void C_SettingPlayBack::setAppContext(AppContext* value) { appContext = value; }

C_SettingPlayBack::C_SettingPlayBack(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
  this->pres = new P_SettingPlayBack(this, zone);
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
void C_SettingPlayBack::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_PLAY_BACK_SET_PATH_SAVE_MEDIA: {
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
void C_SettingPlayBack::newSystemAction(int message, QVariant* attachment) {
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
void C_SettingPlayBack::newAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
    case Message.APP_SHOW:
      break;
    case Message.APP_CONTEXT_GET: {
      this->appContext = attachment->value<AppContext*>();
    } break;

    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
  }
}
