#ifndef C_VWLAYOUTSELECTOR_H
#define C_VWLAYOUTSELECTOR_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_vw_layoutpage.h"
#include "PacModel/control.h"
#include "a_vw_layoutselector.h"
#include "message.h"
#include "p_vw_layoutselector.h"

class C_VWLayoutPage;
class P_VWLayoutSelector;
class A_VWLayoutSelector;

class C_VWLayoutSelector : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_VWLayoutPage* getParent() { return (C_VWLayoutPage*)this->parent; }
  A_VWLayoutSelector* abstraction() { return (A_VWLayoutSelector*)this->abst; }
  P_VWLayoutSelector* presentation() { return (P_VWLayoutSelector*)this->pres; }
  C_VWLayoutSelector(Control* ctrl, QWidget* zone);

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

#endif  // C_VWLAYOUTSELECTOR_H
