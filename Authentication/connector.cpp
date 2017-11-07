#include "connector.h"

#ifdef WIN32
#define BIND_EVENT(IO, EV, FN)         \
  do {                                 \
    socket::event_listener_aux l = FN; \
    IO->on(EV, l);                     \
  } while (0)
#else
#define BIND_EVENT(IO, EV, FN) IO->on(EV, FN)
#define OFF_EVENT(IO, EV) IO->off(EV)
#endif

Connector::Connector(QObject* parent) : QObject(parent), _io(new client()) {
  addSocketListener();
}

void Connector::addSocketListener() {
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  using std::placeholders::_4;
  socket::ptr sock = _io->socket();
  BIND_EVENT(sock, "join",
             std::bind(&Connector::onUserJoined, this, _1, _2, _3, _4));
  BIND_EVENT(sock, "message_data",
             std::bind(&Connector::onUpdateControl, this, _1, _2, _3, _4));
  BIND_EVENT(sock, "CameraOrderChanger",
             std::bind(&Connector::onCamOrderChanger, this, _1, _2, _3, _4));
  _io->set_socket_open_listener(
      std::bind(&Connector::onOpened, this, std::placeholders::_1));
  _io->set_close_listener(std::bind(&Connector::onClosed, this, _1));
  _io->set_fail_listener(std::bind(&Connector::onFailed, this));
}

void Connector::connectToSocket(QString token) {
  qDebug() << Q_FUNC_INFO << token;
  mOptions["token"] = token.toStdString();
  std::string socketUrl =
      AppProfile::getAppProfile()->getAppConfig()->getSocketUrl().toStdString();
  _io->connect(socketUrl, mOptions);
}

void Connector::onOpened(std::string const& nsp) {
  //  qDebug() << Q_FUNC_INFO;
  QString options = mDeviceID.arg(NetworkUtils::instance().getMacAddress());
  QByteArray bytes = options.toUtf8();
  std::string deviceID(bytes.data(), bytes.length());
  _io->socket()->emit("join", deviceID);
  Q_EMIT socketConnected();
}

void Connector::onClosed(client::close_reason const& reason) {
  //  qDebug() << Q_FUNC_INFO;
  Q_EMIT socketDisconnected();
}

void Connector::onFailed() { qDebug() << Q_FUNC_INFO; }

void Connector::onUserJoined(std::string const& name, message::ptr const& data,
                             bool hasAck, message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
  if (data->get_flag() == message::flag_object) {
    std::string msg = data->get_map()["message"]->get_string();
    QString message = QString::fromUtf8(msg.data(), msg.length());
    //    qDebug() << Q_FUNC_INFO << message;
    Q_EMIT socketJoined();
  }
}

void Connector::onUpdateControl(std::string const& name,
                                message::ptr const& data, bool hasAck,
                                message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
}

void Connector::onCamOrderChanger(const string& name, const message::ptr& data,
                                  bool hasAck, message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
  if (data->get_flag() == message::flag_object) {
    //    CamSite* camSite = CamSite::parseSocket(data);
    //    QListWidgetItem* item = new QListWidgetItem(label);
    //    Q_EMIT RequestAddListItem(item);
    //    qDebug() << "socket camera order changer"
    //             << camSite->toString("c4:4e:ac:0d:02:65");
  }
}
