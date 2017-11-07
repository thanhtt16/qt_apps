#ifndef C_VWRIGHTCONTROLBAR_H
#define C_VWRIGHTCONTROLBAR_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "VWLayoutPage/c_vw_layoutpage.h"
#include "p_vw_rightcontrolbar.h"

class C_VWLayoutPage;
class P_VWRightControlBar;
class A_VWRightControlBar;

class C_VWRightControlBar : public Control {
 public:
  QWidget *zone;
  C_VideoWall *getParent() { return (C_VideoWall *)this->parent; }
  P_VWRightControlBar *presentation() { return (P_VWRightControlBar *)pres; }
  A_VWRightControlBar *abstraction() { return (A_VWRightControlBar *)abst; }
  AppContext *appContext = Q_NULLPTR;
  C_VWLayoutPage *cLayoutPage = Q_NULLPTR;
  C_VWRightControlBar(Control *ctrl, QWidget *zone);

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

#endif  // C_VWRIGHTCONTROLBAR_H
