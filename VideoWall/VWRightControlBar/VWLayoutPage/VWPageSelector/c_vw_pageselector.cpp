#include "c_vw_pageselector.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_VWPageSelector::C_VWPageSelector(Control* ctrl, QWidget* zone)
    : Control(ctrl) {
  QVariant* dataStruct = new QVariant();
  getParent()->newAction(Message.APP_CONTEXT_GET, dataStruct);
  this->appContext = dataStruct->value<AppContext*>();

  this->pres = new P_VWPageSelector(this, zone);
  this->zone = zone;
}

void C_VWPageSelector::show(QVariant* attachment) {
  newUserAction(Message.SHOW, attachment);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_VWPageSelector::newUserAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTED: {
      getParent()->newAction(message, attachment);
    } break;

    case Message.APP_VIDEO_WALL_UPDATE_PAGE_DEFAULT: {
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
void C_VWPageSelector::newSystemAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_CONTEXT_GET:
      attachment->setValue(this->appContext);
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
void C_VWPageSelector::newAction(int message, QVariant* attachment) {
  switch (message) {
    case Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTED: {
      // re-calculate pages
      LayoutStruct selectedLayout = attachment->value<LayoutStruct>();
      presentation()->update(selectedLayout);
    } break;

    case Message.APP_VIDEO_WALL_PAGE_DEFAULT_SET: {
      // re-calculate pages
      LayoutStruct selectedLayout = attachment->value<LayoutStruct>();
      presentation()->updateDefaulPage(selectedLayout);
    } break;

    case Message.PAGE_TRANSITION_BEGIN: {
      presentation()->pageTransitionBegin();
    } break;
    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
  }
}
