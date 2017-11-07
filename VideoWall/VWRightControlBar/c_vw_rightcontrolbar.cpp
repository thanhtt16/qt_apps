#include "c_vw_rightcontrolbar.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_VWRightControlBar::C_VWRightControlBar(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
  // update app context
  QVariant* dataStruct = new QVariant();
  parent->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  // init facets
  this->pres = new P_VWRightControlBar(this, zone);
  this->zone = zone;

  cLayoutPage = new C_VWLayoutPage(
      this, presentation()->getZone(Message.APP_VIDEO_WALL_ZONE_LAYOUT_PAGE));
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWRightControlBar::newUserAction(int message, QVariant* attachment) {
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
void C_VWRightControlBar::newSystemAction(int message, QVariant* attachment) {
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
void C_VWRightControlBar::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_VIDEO_WALL_RIGHT_BAR_SHOW:
      presentation()->show(attachment);
      break;

    case Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED:
      this->parent->newAction(message, attachment);
      break;

    case Message.APP_CONTEXT_GET:
      attachment->setValue(this->appContext);
      break;

    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
  }
}
