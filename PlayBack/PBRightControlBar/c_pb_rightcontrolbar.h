#ifndef C_PBRIGHTCONTROLBAR_H
#define C_PBRIGHTCONTROLBAR_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "MainFrame/c_mainframe.h"
#include "PBLayoutPage/c_pb_layoutpage.h"
#include "PacModel/control.h"
#include "p_pb_rightcontrolbar.h"

#include "PBDataMapTab/c_pb_datamaptab.h"
#include "PlayBack/c_playback.h"
class C_PBLayoutPage;
class P_PBRightControlBar;
class A_PBRightControlBar;
class C_PBDataMapTab;
class C_PlayBack;

class C_PBRightControlBar : public Control {
 public:
  QWidget *zone;
  C_PlayBack *getParent() { return (C_PlayBack *)this->parent; }
  P_PBRightControlBar *presentation() { return (P_PBRightControlBar *)pres; }
  A_PBRightControlBar *abstraction() { return (A_PBRightControlBar *)abst; }
  AppContext *appContext = Q_NULLPTR;
  C_PBLayoutPage *cLayoutPage = Q_NULLPTR;
  C_PBDataMapTab *cDataMapTab = Q_NULLPTR;
  C_PBRightControlBar(Control *ctrl, QWidget *zone);

  /**
        * Method to receive a message from the Presentation Facet.
        * @param message    : A string which describe the request
        * @param attachment : A ref on an eventual object necessary to treat the
    *request
        **/
  void newUserAction(int message, QVariant *attachment) override;

  /**
    * Method to receive a message from the Astraction Facet.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newSystemAction(int message, QVariant *attachment) override;

  /**
    * Method to receive a message from an other agent.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
  void newAction(int message, QVariant *attachment) override;
};

#endif  // C_PBRIGHTCONTROLBAR_H
