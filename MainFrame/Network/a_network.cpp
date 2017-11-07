#include "a_network.h"

/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
bool A_Network::getIsConnected() const { return isConnected; }

void A_Network::setIsConnected(bool value) { isConnected = value; }

A_Network::A_Network(Control* ctrl) : Abstraction(ctrl) {
  pingProcess = new QProcess(this);
  connect(pingProcess, &QProcess::readyReadStandardOutput, this,
          &A_Network::onReadyReadStandardOutput);
  connect(pingProcess, &QProcess::readyReadStandardError, this,
          &A_Network::onReadyReadStandardError);
  timerCheckNetwork = new QTimer(this);
  connect(timerCheckNetwork, &QTimer::timeout ,this, &A_Network::recheckNetwork);
  timerCheckNetwork->start(1000);
}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_Network::changeControl(Control* ctrl) { this->ctrl = ctrl; }

void A_Network::recheckNetwork(){
    pingProcess->close();
    startChecking();
}

void A_Network::stopTimerRecheck(){
    if(timerCheckNetwork->isActive()){
        timerCheckNetwork->stop();
        loadDataFirstSuccess = true;
    }
}

void A_Network::startChecking() {
  // get host from api url
  hostPing = AppProfile::getAppProfile()->getAppConfig()->getBaseApiUrl();
  QUrl url = QUrl(hostPing);
  hostPing = url.host();
  // ping with interval 200 ms
  QString exec = "ping";
  QStringList params;
  params << this->hostPing << "-i"
         << "0.5";
  qDebug() << "params network" << params;
  // start pinging
  pingProcess->start(exec, params, QIODevice::ReadOnly);
  QCoreApplication::processEvents();
}



void A_Network::onReadyReadStandardOutput() {
  QByteArray data = pingProcess->readAllStandardOutput();
  QString text(data);
  text.simplified();
  text.trimmed();
//  qDebug() << "+" << text;
  onNetworkIsReachable();
}

void A_Network::onReadyReadStandardError() {
  QByteArray data = pingProcess->readAllStandardError();
  QString text(data);
  text.simplified();
  text.trimmed();
  qDebug() << "-" << text;
  onNetworkIsUnreachable();
}

void A_Network::stopChecking() { pingProcess->close(); }

void A_Network::onNetworkIsReachable() {
  if (!isConnected || !loadDataFirstSuccess) {
    isConnected = true;
    control()->newSystemAction(Message.APP_NETWORK_IS_REACHABLE, Q_NULLPTR);
  }
}

void A_Network::onNetworkIsUnreachable() {
    if (isFirst) {
        isFirst = false;
        if (!isConnected) {
          control()->newSystemAction(Message.APP_NETWORK_IS_UNREACHABLE, Q_NULLPTR);
        }
    } else {
        if (isConnected) {
          isConnected = false;
          control()->newSystemAction(Message.APP_NETWORK_IS_UNREACHABLE, Q_NULLPTR);
        }
    }

}
