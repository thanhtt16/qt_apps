#ifndef C_TOPCONTROLBAR_H
#define C_TOPCONTROLBAR_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "AppSelector/c_appselector.h"
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "ProfileSetting/c_profilesetting.h"
#include "Site/sitechild.h"
#include "SiteSelector/c_siteselector.h"
#include "TopControlBar/a_topcontrolbar.h"
#include "TopControlBar/p_topcontrolbar.h"

class P_TopControlBar;
class A_TopControlBar;
class C_MainFrame;
class C_SiteSelector;
class C_AppSelector;
class C_ProfileSetting;
class C_TopControlBar : public Control {
 public:
  QWidget *zone;
  AppContext *appContext;
  C_SiteSelector *cSiteSelector = Q_NULLPTR;
  C_AppSelector *cAppSelector = Q_NULLPTR;
  C_ProfileSetting *cProfileSetting = Q_NULLPTR;

  A_TopControlBar *abstraction() { return (A_TopControlBar *)this->abst; }
  P_TopControlBar *presentation() { return (P_TopControlBar *)this->pres; }
  C_MainFrame *getParent() { return (C_MainFrame *)this->parent; }
  C_TopControlBar(Control *ctrl, QWidget *zone);

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

#endif  // C_LOGIN_H
