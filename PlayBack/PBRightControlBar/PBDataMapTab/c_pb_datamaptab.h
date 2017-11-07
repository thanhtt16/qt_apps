#ifndef C_PBDATAMAPTAB_H
#define C_PBDATAMAPTAB_H
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "PBPlayBackCalendar/c_pb_calendar.h"
#include "PBPlayListOfDay/c_pb_playlistofday.h"
#include "PacModel/control.h"
#include "a_pb_datamaptab.h"
#include "message.h"
#include "p_pb_datamaptab.h"
class P_PBDataMapTab;
class A_PBDataMapTab;
class C_PBPlayListOfDay;
class C_PBCalendar;
class C_PBDataMapTab : public Control {
 public:
  QWidget* zone;
  A_PBDataMapTab* abstraction() { return (A_PBDataMapTab*)this->abst; }
  P_PBDataMapTab* presentation() { return (P_PBDataMapTab*)this->pres; }
  C_PBDataMapTab(Control* ctrl, QWidget* zone);

  C_PBCalendar* cCalendar = Q_NULLPTR;
  C_PBPlayListOfDay* cPlayListOfDay = Q_NULLPTR;

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

#endif  // C_PBDataMapTab
