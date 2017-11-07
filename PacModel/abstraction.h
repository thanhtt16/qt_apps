#ifndef ABSTRACTION_H
#define ABSTRACTION_H

#include <QObject>
#include "message.h"
class Control;
class Abstraction : public QObject {
  Q_OBJECT
  // A ref on the control facet
 protected:
  Control *ctrl;
  AppMessage Message;

 public:
  Abstraction(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
