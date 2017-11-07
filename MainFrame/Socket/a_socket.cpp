#include "a_socket.h"

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

/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_Socket::A_Socket(Control* ctrl) : Abstraction(ctrl), _io(new client()) {
  addSocketListener();
}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_Socket::changeControl(Control* ctrl) { this->ctrl = ctrl; }

void A_Socket::addSocketListener() {
  using std::placeholders::_1;
  using std::placeholders::_2;
  using std::placeholders::_3;
  using std::placeholders::_4;
  socket::ptr sock = _io->socket();
  BIND_EVENT(sock, "join",
             std::bind(&A_Socket::onUserJoined, this, _1, _2, _3, _4));

  //    BIND_EVENT(sock, "JoinClipRoom",
  //               std::bind(&A_Socket::onJoinClipRoom, this, _1, _2, _3, _4));

  //    BIND_EVENT(sock, "GetClip",
  //               std::bind(&A_Socket::onGetClipSuccess, this, _1, _2, _3, _4));

  BIND_EVENT(sock, "message_data",
             std::bind(&A_Socket::onUpdateControl, this, _1, _2, _3, _4));
  BIND_EVENT(sock, "CameraOrderChanger",
             std::bind(&A_Socket::onCamOrderChanger, this, _1, _2, _3, _4));
  _io->set_socket_open_listener(
        std::bind(&A_Socket::onOpened, this, std::placeholders::_1));
  _io->set_close_listener(std::bind(&A_Socket::onClosed, this, _1));
  _io->set_fail_listener(std::bind(&A_Socket::onFailed, this));
  connect(this, &A_Socket::socketJoined, this, &A_Socket::onJoined);

  //start joinclip room
  //    _io->set_socket_open_listener(
  //                std::bind(&A_Socket::startJoinClipRoom, this, std::placeholders::_1));

}

void A_Socket::connectToSocket() {
  QString token = this->control()->appContext->getWorkingUser()->getToken();
  qDebug() << Q_FUNC_INFO << token;
  mOptions["token"] = token.toStdString();
  std::string socketUrl =
      AppProfile::getAppProfile()->getAppConfig()->getSocketUrl().toStdString();
  _io->connect(socketUrl, mOptions);
  control()->newAction(Message.SHOW_INDICATOR, Q_NULLPTR);
}

void A_Socket::onOpened(std::string const& nsp) {
  //  qDebug() << Q_FUNC_INFO;
  QString options = mDeviceID.arg(NetworkUtils::instance().getMacAddress());
  QByteArray bytes = options.toUtf8();
  std::string deviceID(bytes.data(), bytes.length());
  _io->socket()->emit("join", deviceID);
  Q_EMIT socketConnected();
}


void A_Socket::onClosed(client::close_reason const& reason) {
  //  qDebug() << Q_FUNC_INFO;
  Q_EMIT socketDisconnected();
  control()->newAction(Message.HIDE_INDICATOR, Q_NULLPTR);

}

void A_Socket::onFailed() {
  qDebug() << Q_FUNC_INFO;
  control()->newAction(Message.HIDE_INDICATOR, Q_NULLPTR);
}

void A_Socket::onUserJoined(std::string const& name, message::ptr const& data,
                            bool hasAck, message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
  if (data->get_flag() == message::flag_object) {
      std::string msg = data->get_map()["message"]->get_string();
      QString message = QString::fromUtf8(msg.data(), msg.length());
      //    qDebug() << Q_FUNC_INFO << message;
      Q_EMIT socketJoined();
    }
}

void A_Socket::onJoined() {
  control()->newAction(Message.HIDE_INDICATOR, Q_NULLPTR);
  control()->newSystemAction(Message.APP_SOCKET_CONNECTED, Q_NULLPTR);
}

void A_Socket::onUpdateControl(std::string const& name,
                               message::ptr const& data, bool hasAck,
                               message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
}

void A_Socket::onCamOrderChanger(const string& name, const message::ptr& data,
                                 bool hasAck, message::list& ack_resp) {
  //  qDebug() << Q_FUNC_INFO;
  if (data->get_flag() == message::flag_object) {
      //            CamSite* camSite = CamSite::parseSocket(data);
      //    QListWidgetItem* item = new QListWidgetItem(label);
      //    Q_EMIT RequestAddListItem(item);
      //    qDebug() << "socket camera order changer"
      //             << camSite->toString("c4:4e:ac:0d:02:65");
    }
}


void A_Socket::startJoinClipRoom(std::string const& userId){
  ////“JoinClipRoom”: Request join room in socket server (data: {user_id: 123, type: 'client'});)
  ///
  int userIdWorking = this->control()->appContext->getWorkingUser()->getUserId();
  QString options = joinRoomData.arg(QString::number(userIdWorking));
  qDebug()<<"onJoinClipRoom-Success" << options;
  QByteArray bytes = options.toUtf8();
  std::string Data(bytes.data(), bytes.length());
  _io->socket()->emit("JoinClipRoom",Data);
}

//void A_Socket::startGetClip(std::string const& cameraId,std::string const& cameraName,std::string const& urlVideo){
//    ////“GetClip”: Request trích clip. (data: {"camera_id":3506,"camera_name":"Khu vực cầu thang máy 5",
//    /// "url":"http://2c1.vcam.viettel.vn/rec/hls/d02212d8bb28xyz11381_1506272520000_360000.m3u8"})
//    ///
//    QString options = getClipData.arg(QString::fromStdString(cameraId)).arg(QString::fromStdString(cameraName)).arg(QString::fromStdString(urlVideo));
//    QByteArray bytes = options.toUtf8();
//    std::string DataGetClip(bytes.data(), bytes.length());
//    _io->socket()->emit("GetClip",DataGetClip);
//}

void A_Socket::startGetClip(int cameraId,QString videoName,QString urlVideo){
  ////“GetClip”: Request trích clip. (data: {"camera_id":3506,"camera_name":"Khu vực cầu thang máy 5",
  /// "url":"http://2c1.vcam.viettel.vn/rec/hls/d02212d8bb28xyz11381_1506272520000_360000.m3u8"})
  ///
  //    QString options = getClipData.arg(QString::fromStdString(cameraId)).arg(QString::fromStdString(cameraName)).arg(QString::fromStdString(urlVideo));
  //    QString options = getClipData.arg(QString::number(cameraId)).arg(videoName).arg(urlVideo);
  //    qDebug() << "OPTION GET CLIP " << options;

  urlRecordCurrent = urlVideo;
  QJsonObject jsonObject;
  jsonObject.insert("camera_id",cameraId);
  jsonObject.insert("camera_name", videoName);
  jsonObject.insert("url", urlVideo);
  QJsonDocument document(jsonObject);
  QString strJson(document.toJson(QJsonDocument::Compact));
  qDebug() << "OPTION GET CLIP " << strJson;
  QByteArray bytes = strJson.toUtf8();
  std::string DataGetClip(bytes.data(), bytes.length());
  _io->socket()->emit("GetClip",DataGetClip);
}

void A_Socket::onGetClipSuccess(std::string const &name, message::ptr const &data,
                                bool hasAck, message::list &ack_resp){
  if (data->get_flag() == message::flag_object) {

      int code = data->get_map()["code"]->get_int();
      std::string message = data->get_map()["message"]->get_string();
      if(code == 404)
        {
          control()->newSystemAction(Message.APP_PLAY_BACK_GET_CLIP_RECORD_ERROR, Q_NULLPTR );
        }

      qDebug() << "getClip-Success" << code << QString::fromStdString(message);
      if(code == 0  &&  message == "success"){
          std::string percent = data->get_map()["percent"]->get_string();
          qDebug() << "PERCENT DOWNLOAD" << QString::fromStdString(percent);
          if(QString::fromStdString(percent) == "100"){
              std::string urlMp4 = data->get_map()["url"]->get_string();
              QString url_file_mp4_record = QString::fromUtf8(urlMp4.data(),urlMp4.length());
              QVariant *dataStruct = new QVariant();
              dataStruct->setValue<QString>(url_file_mp4_record);
              control()->newSystemAction(Message.APP_PLAY_BACK_GET_CLIP_RECORD_SUCCESS,dataStruct);
            }
        }
    }

}

void A_Socket::cancelGetClip(){
  QJsonObject jsonObject;
  jsonObject.insert("url",urlRecordCurrent);
  QJsonDocument document(jsonObject);
  QString strJson(document.toJson(QJsonDocument::Compact));
  QByteArray bytes = strJson.toUtf8();
  std::string DataCancelGetClip(bytes.data(), bytes.length());
  _io->socket()->emit("GetClip",DataCancelGetClip);
}



void A_Socket::onJoinClipRoom(std::string const &name, message::ptr const &data,
                              bool hasAck, message::list &ack_resp){
  qDebug() << "onJoinClipRoom-Success";

  if (data->get_flag() == message::flag_object) {
      std::string name = data->get_map()["message"]->get_string();
      QString message = QString::fromUtf8(name.data(), name.length());
      qDebug() << Q_FUNC_INFO << message;
    }
}
