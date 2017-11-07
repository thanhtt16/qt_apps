#ifndef C_CLASSNAME_H
#define C_CLASSNAME_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "PacAgentTemplate/a_classname.h"
#include "PacAgentTemplate/p_classname.h"
#include "PacModel/control.h"
#include "message.h"
class P_ClassName;
class A_ClassName;
class C_ClassName : public Control {
  QWidget* zone;

 public:
  C_ClassName(Control* ctrl, QWidget* zone);

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
