#ifndef C_PBPLAYLISTOFDAY_H
#define C_PBPLAYLISTOFDAY_H

#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "../c_pb_datamaptab.h"
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.h"
#include "a_pb_playlistofday.h"
#include "message.h"
#include "p_pb_playlistofday.h"
class P_PBPlayListOfDay;
class A_PBPlayListOfDay;
class C_PBDataMapTab;
class C_PBLayoutPage;
class C_PBPlayListOfDay : public Control {
  QWidget* zone;

 public:
  AppContext* appContext;

  C_PBPlayListOfDay(Control* ctrl, QWidget* zone);
  C_PBLayoutPage* getParent() { return (C_PBLayoutPage*)this->parent; }

  P_PBPlayListOfDay* presentation() { return (P_PBPlayListOfDay*)this->pres; }
  A_PBPlayListOfDay* abstraction() { return (A_PBPlayListOfDay*)this->abst; }
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

#endif  // C_PBPLAYLISTOFDAY_H
