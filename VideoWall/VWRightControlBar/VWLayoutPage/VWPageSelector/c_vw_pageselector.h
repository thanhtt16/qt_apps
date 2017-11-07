#ifndef A_VWPageSelector_H
#define A_VWPageSelector_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_vw_layoutpage.h"
#include "PacModel/control.h"
#include "a_vw_pageselector.h"
#include "message.h"
#include "p_vw_pageselector.h"

class P_VWPageSelector;
class A_VWPageSelector;

class C_VWPageSelector : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_VWLayoutPage* getParent() { return (C_VWLayoutPage*)this->parent; }
  A_VWPageSelector* abstraction() { return (A_VWPageSelector*)this->abst; }
  P_VWPageSelector* presentation() { return (P_VWPageSelector*)this->pres; }
  C_VWPageSelector(Control* ctrl, QWidget* zone);

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

#endif  // A_VWPageSelector_H
