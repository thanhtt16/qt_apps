#ifndef P_NETWORK_H
#define P_NETWORK_H

#include <PacModel/presentation.h>
#include <QApplication>
#include <QMessageBox>
#include <QObject>
#include "c_network.h"

class C_Network;

class P_Network : public Presentation {
 private:
  QWidget *zone;
  QMessageBox messageBox;

 public:
  P_Network(Control *ctrl, QWidget *zone);
  C_Network *control() { return (C_Network *)(this->ctrl); }

  void changeControl(Control *ctrl);
  void update();

  void showDialog(QString message);
  void hideDialog();

  QObject *getZone(int zoneId);
};

#endif  // P_NETWORK_H
