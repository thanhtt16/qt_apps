#ifndef P_CLASSNAME_H
#define P_CLASSNAME_H

#include <PacModel/presentation.h>
#include <QObject>
class C_ClassName;
class P_ClassName : public Presentation {
  // init ui control
 private:
 public:
  P_ClassName(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update();

  QObject *getZone(int zoneId);
};

#endif  // PRESENTATION_H
