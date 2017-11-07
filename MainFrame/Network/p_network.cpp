#include "p_network.h"

/**
     * Generic method to override for updating the presention.
     **/

P_Network::P_Network(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
  this->zone = zone;
  messageBox.setButtonText(1, "Thoát");
  messageBox.setStyleSheet(
      "QLabel{min-width:300 px; font-size: 14px;} QPushButton{ width:100px; "
      "font-size: 12px; }");
}

void P_Network::showDialog(QString message) {
//  messageBox.setText(message);
//  int result = messageBox.exec();
//  if (result == QMessageBox::Ok) {
//    if (control()->abstraction()->getIsConnected()) {
//    } else {
//      qDebug() << Q_FUNC_INFO;
//      qApp->exit();
//      QProcess process;
//      process.execute(QString("kill %1").arg(
//          QApplication::QCoreApplication::applicationPid()));
//    }
//  }
}

void P_Network::hideDialog() { messageBox.close(); }

void P_Network::update() {}

QObject *P_Network::getZone(int zoneId) {
  switch (zoneId) {
    case 1:
      return Q_NULLPTR;
    default:
      return Q_NULLPTR;
  }
}
