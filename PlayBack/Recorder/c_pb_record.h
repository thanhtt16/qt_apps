#ifndef C_PBRecord_H
#define C_PBRecord_H
#include <QString>

#include <PlayBack/Recorder/a_pb_record.h>
#include <PlayBack/Recorder/p_pb_record.h>
#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "PacModel/control.h"
#include "PlayBack/PBWorkSpace/c_pb_workspace.h"
#include "message.h"
class P_PBRecord;
class A_PBRecord;
class C_PBWorkSpace;
class C_PBRecord : public Control {
  QWidget* zone;

 public:
  C_PBRecord(Control* ctrl, QWidget* zone);
  C_PBWorkSpace* getParent() { return (C_PBWorkSpace*)this->parent; }
  P_PBRecord* presentation() { return (P_PBRecord*)pres; }
  A_PBRecord* abstraction() { return (A_PBRecord*)abst; }
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
