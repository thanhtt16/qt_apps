#ifndef C_SITESELECTOR_H
#define C_SITESELECTOR_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "TopControlBar/c_topcontrolbar.h"
#include "VideoWall/VWWorkSpace/c_vw_workspace.h"
#include "a_siteselector.h"
#include "message.h"
#include "p_siteselector.h"
class P_SiteSelector;
class A_SiteSelector;
class C_VWWorkSpace;
class C_TopControlBar;
class C_SiteSelector : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_SiteSelector(Control* ctrl, QWidget* zone);
  C_TopControlBar* getParent() { return (C_TopControlBar*)this->parent; }
  P_SiteSelector* presentation() { return (P_SiteSelector*)pres; }
  A_SiteSelector* abstraction() { return (A_SiteSelector*)abst; }
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

#endif  // C_SITESELECTOR_H
