#include "c_vw_layoutpage.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_VWLayoutPage::C_VWLayoutPage(Control* ctrl, QWidget* zone) : Control(ctrl) {
  QVariant* dataStruct = new QVariant();
  getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  this->pres = new P_VWLayoutPage(this, zone);
  this->zone = zone;

  cLayoutSelector = new C_VWLayoutSelector(
      this,
      presentation()->getZone(Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTOR));

  cPageSelector = new C_VWPageSelector(
      this, presentation()->getZone(Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTOR));

}

void C_VWLayoutPage::show(QVariant* attachment) { Q_UNUSED(attachment) }

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWLayoutPage::newUserAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
    case Message.SHOW:
      break;

    case Message.APP_SHOW_SETTINGS: {
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
void C_VWLayoutPage::newSystemAction(int message, QVariant* attachment) {
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
void C_VWLayoutPage::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED: {
      this->cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_PAGE_DEFAULT_SET: {
      this->cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_CONTEXT_GET:
      attachment->setValue(this->appContext);
      break;

    case Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED:
      getParent()->newAction(message, attachment);
      break;

    case Message.APP_VIDEO_WALL_LAYOUT_DEFAULT_SET:
      cLayoutSelector->newAction(message, attachment);
      break;

    case Message.SITE_CHANGED: {
      cLayoutSelector->newAction(message, attachment);
    } break;

    case Message.PAGE_TRANSITION_BEGIN: {
      cPageSelector->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT: {
      getParent()->newAction(message, attachment);
    } break;

    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
  }
}
