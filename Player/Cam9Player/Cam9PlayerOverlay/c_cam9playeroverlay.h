#ifndef C_CAM9PLAYEROVERLAY_H
#define C_CAM9PLAYEROVERLAY_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_cam9player.h"
#include "PacModel/control.h"
#include "a_cam9playeroverlay.h"
#include "message.h"
#include "p_cam9playeroverlay.h"

class C_Cam9Player;
class P_Cam9PlayerOverlay;
class A_Cam9PlayerOverlay;

class C_Cam9PlayerOverlay : public Control {
  QWidget* zone;

 public:
  C_Cam9Player* getParent() { return (C_Cam9Player*)this->parent; }

  A_Cam9PlayerOverlay* abstraction() {
    return (A_Cam9PlayerOverlay*)this->abst;
  }

  P_Cam9PlayerOverlay* presentation() {
    return (P_Cam9PlayerOverlay*)this->pres;
  }

  C_Cam9PlayerOverlay(Control* ctrl, QWidget* zone);

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

#endif  // C_CAM9PLAYEROVERLAY_H
