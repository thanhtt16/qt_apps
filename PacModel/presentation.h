#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <QObject>
#include "message.h"
class Control;
class Presentation : public QObject {
  Q_OBJECT
 protected:
  AppMessage Message;
  AppSize appSize;

 public:
  // A Ref on my controler
  Control *ctrl = Q_NULLPTR;

  Presentation(Control *ctrl);
  void changeControl(Control *ctrl);
  void update();
};

#endif  // PRESENTATION_H
