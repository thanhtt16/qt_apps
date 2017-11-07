#ifndef C_VWPAGECONTROLTAB_H
#define C_VWPAGECONTROLTAB_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "../../c_videowall.h"
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "VWLayoutSelector/c_vw_layoutselector.h"
#include "VWPageSelector/c_vw_pageselector.h"
#include "a_vw_layoutpage.h"
#include "message.h"
#include "p_vw_layoutpage.h"

class C_VideoWall;
class C_VWLayoutSelector;
class C_VWPageSelector;

class P_VWLayoutPage;
class A_VWLayoutPage;

class C_VWLayoutPage : public Control {
 public:
  QWidget* zone;
  AppContext* appContext;
  C_VWLayoutSelector* cLayoutSelector = Q_NULLPTR;
  C_VWPageSelector* cPageSelector = Q_NULLPTR;
  C_VideoWall* getParent() { return (C_VideoWall*)this->parent; }
  A_VWLayoutPage* abstraction() { return (A_VWLayoutPage*)this->abst; }
  P_VWLayoutPage* presentation() { return (P_VWLayoutPage*)this->pres; }
  C_VWLayoutPage(Control* ctrl, QWidget* zone);

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

#endif  // C_VWPAGECONTROLTAB_H
