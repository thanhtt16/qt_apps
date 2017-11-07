#ifndef C_VIDEOWALL_H
#define C_VIDEOWALL_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "VWRightControlBar/c_vw_rightcontrolbar.h"
#include "VWWorkSpace/c_vw_workspace.h"
#include "VideoWall/a_videowall.h"
#include "VideoWall/p_videowall.h"
#include "message.h"

class P_VideoWall;
class A_VideoWall;
class C_VWRightControlBar;
class C_VWLayoutPage;
class C_VWWorkSpace;
class C_MainFrame;

class C_VideoWall : public Control {
 public:
  QString appName = "Video Wall";
  QWidget *zone;
  AppContext *appContext;

  C_MainFrame *getParent() { return (C_MainFrame *)this->parent; }
  P_VideoWall *presentation() { return (P_VideoWall *)pres; }
  A_VideoWall *abstraction() { return (A_VideoWall *)abst; }

  C_VWWorkSpace *cWorkspace = Q_NULLPTR;
  C_VWRightControlBar *cRightControlBar = Q_NULLPTR;
  C_VWLayoutPage *cLayoutPage = Q_NULLPTR;

  C_VideoWall(Control *ctrl, QWidget *zone);

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

#endif  // CONTROL_H
