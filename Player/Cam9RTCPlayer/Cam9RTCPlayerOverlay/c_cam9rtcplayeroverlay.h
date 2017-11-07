#ifndef C_Cam9RTCPlayerOverlay_H
#define C_Cam9RTCPlayerOverlay_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_cam9rtcplayer.h"
#include "PacModel/control.h"
#include "a_cam9rtcplayeroverlay.h"
#include "message.h"
#include "p_cam9rtcplayeroverlay.h"

class C_Cam9RTCPlayer;
class P_Cam9RTCPlayerOverlay;
class A_Cam9RTCPlayerOverlay;

class C_Cam9RTCPlayerOverlay : public Control {
  QWidget* zone;

 public:
  C_Cam9RTCPlayer* getParent() { return (C_Cam9RTCPlayer*)this->parent; }

  A_Cam9RTCPlayerOverlay* abstraction() {
    return (A_Cam9RTCPlayerOverlay*)this->abst;
  }

  P_Cam9RTCPlayerOverlay* presentation() {
    return (P_Cam9RTCPlayerOverlay*)this->pres;
  }

  C_Cam9RTCPlayerOverlay(Control* ctrl, QWidget* zone);

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

#endif  // C_Cam9RTCPlayerOverlay_H
