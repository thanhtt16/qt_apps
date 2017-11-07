#ifndef P_LOGIN_H
#define P_LOGIN_H

#include <PacModel/presentation.h>
#include <QObject>
#include "Login/loginwidget.h"

class C_Login;
class P_Login : public Presentation {
  // init ui control
 private:
  QWidget *zone = Q_NULLPTR;
  LoginWidget *loginWindow = Q_NULLPTR;

 public:
  P_Login(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update();
  void show(QVariant *attachment);
  void hide();

  QObject *getZone(int zoneId);
};

#endif  // P_LOGIN_H
