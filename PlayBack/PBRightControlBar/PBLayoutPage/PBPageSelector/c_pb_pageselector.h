#ifndef A_PBPageSelector_H
#define A_PBPageSelector_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_pb_layoutpage.h"
#include "PacModel/control.h"
#include "a_pb_pageselector.h"
#include "message.h"
#include "p_pb_pageselector.h"

class P_PBPageSelector;
class A_PBPageSelector;
class C_PBLayoutPage;
class C_PBPageSelector : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_PBLayoutPage* getParent() { return (C_PBLayoutPage*)this->parent; }
  A_PBPageSelector* abstraction() { return (A_PBPageSelector*)this->abst; }
  P_PBPageSelector* presentation() { return (P_PBPageSelector*)this->pres; }
  C_PBPageSelector(Control* ctrl, QWidget* zone);

  void show(QVariant* attachment);
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

#endif  // A_PBPageSelector_H
