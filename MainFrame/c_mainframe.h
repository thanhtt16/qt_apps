#ifndef C_MAINFRAME_H
#define C_MAINFRAME_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include <QDateTime>


#include "Authentication/appcontext.h"
#include "Login/c_login.h"
#include "PacModel/control.h"
#include "PlayBack/c_playback.h"
#include "Settings/c_settings.h"
#include "TopControlBar/c_topcontrolbar.h"
#include "VideoWall/c_videowall.h"
#include "a_mainframe.h"
#include "message.h"
#include "p_mainframe.h"
#include "Socket/c_socket.h"
#include "Network/c_network.h"
#include <AppSettings/appsetttings.h>
#include <QSettings>
#include "AboutApp/c_about.h"
class P_MainFrame;
class A_MainFrame;
class C_TopControlBar;
class C_VideoWall;
class C_PlayBack;
class C_Settings;
class C_Network;
class C_About;
class C_MainFrame : public Control {
  Q_OBJECT
 public:
  A_MainFrame *abstraction() { return (A_MainFrame *)this->abst; }
  P_MainFrame *presentation() { return (P_MainFrame *)this->pres; }

 private:
  QTimer *timer;
  QDateTime currentDateSelected;
  QWidget *zone;
  CamSite *camSite = Q_NULLPTR;
  C_TopControlBar *cTopControlBarOverlay = Q_NULLPTR;
  C_TopControlBar *cTopControlBar = Q_NULLPTR;
  C_VideoWall *cVideoWall = Q_NULLPTR;
  C_PlayBack *cPlayBack = Q_NULLPTR;
  C_VideoWall *cMagicShow = Q_NULLPTR;
  C_Login *cLogin = Q_NULLPTR;
  C_Settings *cSettings = Q_NULLPTR;
  C_Socket *cSocket = Q_NULLPTR;
  C_Network *cNetwork = Q_NULLPTR;
  C_About *cAboutApp = Q_NULLPTR;

 public:
  AppContext *appContext = Q_NULLPTR;
  C_MainFrame(Control *ctrl, QWidget *zone);
  //  void getSiteOfWorker(QVariant *attachment);
  //  void getWorkingSite(QVariant *attachment);
  AppContext *getAppContext();
  void setAppContext(AppContext *value);
  void timeUpdate();
  int countTimePlayListOfDay = 0;
  int countTimeDelayTransition = 0;
  int modePageTransition =  -1;
  int timeDelayTrasition = -1;
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
  void newAction(int message, QVariant *attachment);
};

#endif  // C_MAINFRAME_H
