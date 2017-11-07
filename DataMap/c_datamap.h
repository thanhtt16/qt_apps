#ifndef C_DATAMAP_H
#define C_DATAMAP_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"
#include "a_datamap.h"
#include "message.h"
#include "p_datamap.h"

class P_DataMap;
class A_DataMap;

class C_DataMap : public Control {
 private:
  QWidget* zone;

 public:
  C_DataMap(Control* ctrl, QWidget* zone);
  A_DataMap* abstraction() { return (A_DataMap*)this->abst; }
  P_DataMap* presentation() { return (P_DataMap*)this->pres; }
  /**
        * Method to receive a message from the Presentation Facet.
        * @param message    : A string which describe the request
        * @param attachment : A ref on an eventual object necessary to treat the
    *request
        **/
  void newUserAction(int message, QVariant* attachment) override;
  /**
    * Method to receive a message from the Astraction Facet.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newSystemAction(int message, QVariant* attachment) override;

  /**
    * Method to receive a message from an other agent.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newAction(int message, QVariant* attachment) override;
};

#endif  // C_DATAMAP_H
