#include "Login/p_login.h"

/**
     * Generic method to override for updating the presention.
     **/

P_Login::P_Login(Control* ctrl, QWidget* zone) : Presentation(ctrl) {
  this->zone = zone;
}

void P_Login::show(QVariant* attachment) {
  if (loginWindow == Q_NULLPTR) {
    loginWindow = new LoginWidget(Q_NULLPTR, attachment);
  } else {
  }
  loginWindow->exec();
}

void P_Login::hide() { loginWindow->hide(); }

void P_Login::update() {}

QObject* P_Login::getZone(int zoneId) {
  switch (zoneId) {
    case 1:
      return Q_NULLPTR;
    default:
      return Q_NULLPTR;
  }
}
