#ifndef C_PBCAlENDAR_H
#define C_PBCAlENDAR_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "PlayBack/PBRightControlBar/PBDataMapTab/c_pb_datamaptab.h"
#include "PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.h"
#include "a_pb_calendar.h"
#include "message.h"
#include "p_pb_calendar.h"
class P_PBCalendar;
class A_PBCalendar;
class C_PBDataMapTab;
class C_PBLayoutPage;
class C_PBCalendar : public Control {
  QWidget* zone;

 public:
  AppContext* appContext;

  C_PBCalendar(Control* ctrl, QWidget* zone);
  C_PBLayoutPage* getParent() { return (C_PBLayoutPage*)this->parent; }
  P_PBCalendar* presentation() { return (P_PBCalendar*)pres; }
  A_PBCalendar* abstraction() { return (A_PBCalendar*)abst; }
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

#endif  // CONTROL_H
