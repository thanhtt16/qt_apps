#include "c_datamap.h"

/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
C_DataMap::C_DataMap(Control* ctrl, QWidget* zone) : Control(ctrl) {
  this->zone = zone;

  this->abst = new A_DataMap(this);
  this->pres = new P_DataMap(this, this->zone);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_DataMap::newUserAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
//    case Message.APP_PLAY_BACK_DATA_MAP_FETCH:
//      newAction(message, attachment);
//      break;

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
void C_DataMap::newSystemAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
//    case Message.APP_PLAY_BACK_DATA_MAP_DRAW: {
//      DataMapArray* dataMapArray = attachment->value<DataMapArray*>();
//      presentation()->setDataMapArray(dataMapArray);
//    } break;

    default:
      qDebug() << "ERROR : General System action in" << getClass()
               << "non-catched :" + Message.toString(message);
      break;
  }
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void C_DataMap::newAction(int message, QVariant* attachment) {
  Q_UNUSED(attachment)
  switch (message) {
//    case Message.APP_PLAY_BACK_DATA_MAP_FETCH: {
//      QString dataUrl = attachment->value<QString>();
//      abstraction()->loadJsonFrom(dataUrl);
//    } break;

    default:
      qDebug() << "ERROR : General Internal pac action in" << getClass()
               << "non-catched :" + Message.toString(message);
      break;
  }
}
