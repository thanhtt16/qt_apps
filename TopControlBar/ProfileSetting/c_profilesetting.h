#ifndef C_ProfileSetting_H
#define C_ProfileSetting_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "TopControlBar/c_topcontrolbar.h"
#include "VideoWall/VWWorkSpace/c_vw_workspace.h"
#include "a_profilesetting.h"
#include "message.h"
#include "p_profilesetting.h"
class P_ProfileSetting;
class A_ProfileSetting;
class C_VWWorkSpace;
class C_TopControlBar;
class C_ProfileSetting : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_ProfileSetting(Control* ctrl, QWidget* zone);
  C_TopControlBar* getParent() { return (C_TopControlBar*)this->parent; }
  P_ProfileSetting* presentation() { return (P_ProfileSetting*)pres; }
  A_ProfileSetting* abstraction() { return (A_ProfileSetting*)abst; }
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

#endif  // C_ProfileSetting_H
