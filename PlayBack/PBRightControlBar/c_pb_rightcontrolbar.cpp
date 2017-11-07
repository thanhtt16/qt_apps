#include "c_pb_rightcontrolbar.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_PBRightControlBar::C_PBRightControlBar(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
  // update app context
  QVariant* dataStruct = new QVariant();
  parent->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  // init facets
  this->pres = new P_PBRightControlBar(this, zone);
  this->zone = zone;

  cLayoutPage = new C_PBLayoutPage(
      this, presentation()->getZone(Message.APP_PLAY_BACK_ZONE_LAYOUT_PAGE));
  //  cLayoutPage->newAction(Message.APP_PLAY_BACK_LAYOUT_DEFAULT_SET,
  //  Q_NULLPTR);
  cDataMapTab = new C_PBDataMapTab(
      this, presentation()->getZone(Message.APP_PLAY_BACK_ZONE_DATA_MAP));
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_PBRightControlBar::newUserAction(int message, QVariant* attachment) {
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
void C_PBRightControlBar::newSystemAction(int message, QVariant* attachment) {
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
void C_PBRightControlBar::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_PLAY_BACK_RIGHT_BAR_SHOW:
      presentation()->show(attachment);
      break;
    case Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION:
      getParent()->newAction(message, attachment);
      break;
    case Message.APP_PLAY_BACK_ZONE_LAYOUT_SELECTED:
      getParent()->newAction(message, attachment);
      break;
    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTED:
      getParent()->newAction(message, attachment);
    case Message.APP_CONTEXT_GET:
      attachment->setValue(this->appContext);
      break;
    case Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED:
      getParent()->newAction(message, attachment);
      break;

    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
  }
}
