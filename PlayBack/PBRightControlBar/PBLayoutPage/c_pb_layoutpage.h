#ifndef C_PBPAGECONTROLTAB_H
#define C_PBPAGECONTROLTAB_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "../PBDataMapTab/PBPlayBackCalendar/c_pb_calendar.h"
#include "../PBDataMapTab/PBPlayListOfDay/c_pb_playlistofday.h"
#include "Authentication/appcontext.h"
#include "PBLayoutSelector/c_pb_layoutselector.h"
#include "PBPageSelector/c_pb_pageselector.h"
#include "PacModel/control.h"
#include "PlayBack/PBRightControlBar/c_pb_rightcontrolbar.h"
#include "PlayBack/c_playback.h"
#include "a_pb_layoutpage.h"
#include "message.h"
#include "p_pb_layoutpage.h"
class C_PBRightControlBar;
class C_PBLayoutSelector;
class C_PBPageSelector;

class P_PBLayoutPage;
class A_PBLayoutPage;
class C_PBCalendar;
class C_PBPlayListOfDay;
class C_PlayBack;
class C_PBLayoutPage : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_PBLayoutSelector* cLayoutSelector = Q_NULLPTR;
  C_PBPageSelector* cPageSelector = Q_NULLPTR;
  C_PBCalendar* cCalendar = Q_NULLPTR;
  C_PBPlayListOfDay* cPlayListOfDay = Q_NULLPTR;

  C_PlayBack* getParent() { return (C_PlayBack*)this->parent; }
  A_PBLayoutPage* abstraction() { return (A_PBLayoutPage*)this->abst; }
  P_PBLayoutPage* presentation() { return (P_PBLayoutPage*)this->pres; }
  C_PBLayoutPage(Control* ctrl, QWidget* zone);

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

#endif  // C_PBPAGECONTROLTAB_H
