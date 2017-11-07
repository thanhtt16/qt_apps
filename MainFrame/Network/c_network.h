#ifndef C_NETWORK_H
#define C_NETWORK_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>

#include "Authentication/appcontext.h"
#include "MainFrame/c_mainframe.h"
#include "PacModel/control.h"
#include "a_network.h"
#include "message.h"
#include "p_network.h"

class P_Network;
class A_Network;
class C_MainFrame;

class C_Network : public Control {
  QWidget* zone;

 public:
  AppContext* appContext = Q_NULLPTR;
  C_Network(Control* ctrl, QWidget* zone);

  P_Network* presentation() { return (P_Network*)this->pres; }
  A_Network* abstraction() { return (A_Network*)this->abst; }

  C_MainFrame* getParent() { return (C_MainFrame*)this->parent; }

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

#endif  // CONTROL_H
