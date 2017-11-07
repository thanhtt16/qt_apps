#ifndef C_PBLAYOUTSELECTOR_H
#define C_PBLAYOUTSELECTOR_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"
#include "PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.h"
#include "a_pb_layoutselector.h"
#include "message.h"
#include "p_pb_layoutselector.h"

class C_PBLayoutPage;
class P_PBLayoutSelector;
class A_PBLayoutSelector;

class C_PBLayoutSelector : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_PBLayoutPage* getParent() { return (C_PBLayoutPage*)this->parent; }
  A_PBLayoutSelector* abstraction() { return (A_PBLayoutSelector*)this->abst; }
  P_PBLayoutSelector* presentation() { return (P_PBLayoutSelector*)this->pres; }
  C_PBLayoutSelector(Control* ctrl, QWidget* zone);

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

#endif  // C_PBLAYOUTSELECTOR_H
