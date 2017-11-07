#ifndef C_LOGIN_H
#define C_LOGIN_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"
#include "a_login.h"
#include "message.h"
#include "p_login.h"
class P_Login;
class A_Login;
class C_Login : public Control {
  Q_OBJECT
 private:
  QWidget* zone;

 public:
  C_Login(Control* ctrl, QWidget* zone);
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

#endif  // C_LOGIN_H
