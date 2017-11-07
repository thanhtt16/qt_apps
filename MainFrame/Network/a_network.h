#ifndef A_NETWORK_H
#define A_NETWORK_H

#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QProcess>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QThread>
#include <QUrl>
#include "Common/appprofile.h"
#include "c_network.h"
#include <QTimer>

class C_Network;

class A_Network : public Abstraction {
  Q_OBJECT
 private:
  QProcess *pingProcess;
  QString hostPing = "www.google.com";
  bool isConnected = false;
  bool isFirst = true;
  bool loadDataFirstSuccess = false;
 public:
  QTimer *timerCheckNetwork;
  C_Network *control() { return (C_Network *)ctrl; }
  A_Network(Control *ctrl);
  void changeControl(Control *ctrl);

  void stopChecking();
  void startChecking();

  void onNetworkIsReachable();
  void onNetworkIsUnreachable();


  bool getIsConnected() const;
  void setIsConnected(bool value);
  void stopTimerRecheck();
 public Q_SLOTS:
  void recheckNetwork();
  void onReadyReadStandardOutput();
  void onReadyReadStandardError();
};

#endif  // A_NETWORK_H
