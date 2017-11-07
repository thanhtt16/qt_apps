#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <sio_client.h>
#include <QObject>
#include "Authentication/user.h"
#include "Camera/camitem.h"
#include "Common/appprofile.h"
#include "Common/networkutils.h"
using namespace sio;
using namespace std;
class CamItem;
class AppProfile;
class Connector : public QObject {
  Q_OBJECT
 public:
  explicit Connector(QObject *parent = 0);
  std::unique_ptr<client> _io;
  std::map<std::string, std::string> mOptions;
  QString mDeviceID = "{\"device_id\":\"%1\"}";
  void addSocketListener();
  void connectToSocket(QString token);
 public Q_SLOTS:
  void onOpened(std::string const &nsp);
  void onClosed(client::close_reason const &reason);
  void onFailed();
  void onUserJoined(std::string const &name, message::ptr const &data,
                    bool hasAck, message::list &ack_resp);
  void onUpdateControl(std::string const &name, message::ptr const &data,
                       bool hasAck, message::list &ack_resp);
  void onCamOrderChanger(const string &name, const message::ptr &data,
                         bool hasAck, message::list &ack_resp);
 Q_SIGNALS:
  void socketConnected();
  void socketDisconnected();
  void socketJoined();
};

#endif  // CONNECTOR_H
