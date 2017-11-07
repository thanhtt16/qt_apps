#ifndef C_PlayBack_H
#define C_PlayBack_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "MainFrame/c_mainframe.h"
#include "PBRightControlBar/c_pb_rightcontrolbar.h"
#include "PBWorkSpace/c_pb_workspace.h"
#include "PacModel/control.h"
#include "PlayBack/PBRightControlBar/PBLayoutPage/c_pb_layoutpage.h"
#include "PlayBack/a_playback.h"
#include "PlayBack/p_playback.h"
#include "message.h"

class P_PlayBack;
class A_PlayBack;
class C_PBRightControlBar;
class C_PBLayoutPage;
class C_PBWorkSpace;
class C_PBLayoutPage;
class C_MainFrame;

class C_PlayBack : public Control {
 public:
  QString appName = "Playback";
  QWidget *zone;
  AppContext *appContext;

  C_MainFrame *getParent() { return (C_MainFrame *)this->parent; }
  P_PlayBack *presentation() { return (P_PlayBack *)this->pres; }
  A_PlayBack *abstraction() { return (A_PlayBack *)this->abst; }

  C_PBWorkSpace *cWorkspace = Q_NULLPTR;
  C_PBLayoutPage *cLayoutPage = Q_NULLPTR;

  C_PlayBack(Control *ctrl, QWidget *zone);

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
