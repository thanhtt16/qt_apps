#ifndef C_PBSeekBar_H
#define C_PBSeekBar_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"

#include "PlayBack/PBWorkSpace/c_pb_workspace.h"
#include "a_pb_seekbar.h"
#include "message.h"
#include "p_pb_seekbar.h"
class P_PBSeekBar;
class A_PBSeekBar;
class C_PBWorkSpace;
class C_PBSeekBar : public Control {
  QWidget* zone;

 public:
  C_PBSeekBar(Control* ctrl, QWidget* zone);
  C_PBWorkSpace* getParent() { return (C_PBWorkSpace*)this->parent; }
  P_PBSeekBar* presentation() { return (P_PBSeekBar*)pres; }
  A_PBSeekBar* abstraction() { return (A_PBSeekBar*)abst; }
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
