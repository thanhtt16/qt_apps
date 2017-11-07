#include "RTCSocket.h"

RTCSocket::RTCSocket(RTCPlayer *rtcPlayer) : pRTCPlayer(rtcPlayer) {
  socket = new QWebSocket();
  connect(socket, &QWebSocket::connected, this, &RTCSocket::onSocketConnected);
  connect(socket, &QWebSocket::disconnected, this, &RTCSocket::onSocketDisconnected);
  connect(socket, &QWebSocket::textMessageReceived, this, &RTCSocket::onRawPackageIndexReceived);

  //reconnectTimer = new QTimer(this);
  //connect(reconnectTimer, &QTimer::timeout, this, &RTCSocket::onReconnonSocketDisconnectedectSocket);

  pRTCDownloader = pRTCPlayer->getRtpDownloader();
}

RTCSocket::~RTCSocket() {
  socket->deleteLater();
}

RTCPlayer *RTCSocket::getPRTCPlayer() const
{
  return pRTCPlayer;
}

void RTCSocket::openSource(QString source) {
  if (socket->isValid() && (this->socketUrl == source)) return;
  setNewSource(source);
  isSecondIdrReached = true;
  openSocket();
}

void RTCSocket::setNewSource(QString source) {
  socketUrl = source;
  //buildLinkFormat for download link
  QString tmpUrl = socketUrl;
  tmpUrl.replace("//", "/");
  QStringList splitedUrl = tmpUrl.split("/");
  if (splitedUrl.length() == 5) {
      host = splitedUrl.at(1);
      mac = splitedUrl.at(3);
      cam = splitedUrl.at(4);
    }
  linkFormat = QString("http://%1/live/g/%2/%3").arg(host).arg(cam).arg("%1");
}


void RTCSocket::openSocket() {
  QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  qDebug() << playerName << "::RTCSocket::openSocket" ;
  socket->close();
  if (!socketUrl.isEmpty()) {
      if (!isNetworkReachable) { return; }
      socket->open(QUrl(socketUrl));
  }
}

void RTCSocket::onSocketConnected() {
  QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  qDebug() << playerName << "::RTCSocket::onSocketConnected" ;
  sendSignature();
  //Q_EMIT socketConnected();
}

void RTCSocket::sendSignature() {
  QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  qDebug() << playerName << "::RTCSocket::sendSignature" ;

  Cam9MapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
  rtpQueue.empty();

  playerIsPlaying = true;

  QString signature = "{ \"action\":\"hello\", "
                      "\"version\":\"2.0\", "
                      "\"host_id\":\"%1\", "
                      "\"signature\":\"RESERVED\", "
                      "\"timestamp\":\"%2\" }";
  signature = signature.arg(mac).arg(QDateTime::currentMSecsSinceEpoch());
  socket->sendTextMessage(signature);
  socketState = RTCSS_SentSignature;
}

void RTCSocket::onRawPackageIndexReceived(const QString &message) {
  switch (socketState) {
    case RTCSS_SentSignature: {
        socketState = RTCSS_ReceivedSignature;
        linkIndex = 0;
        //onReceivedGopPackage(message);
        //qDebug() << "RTCSocket::" << message;
      } break;
    case RTCSS_ReceivedSignature: {
        socketState = RTCSS_ReceivedDownloadLink;
        //qDebug() << "RTCSocket::Index[" << linkIndex << "]=" << message;
        prepareLinkToDownload(message);
      } break;
    case RTCSS_ReceivedDownloadLink: {
        prepareLinkToDownload(message);
        //qDebug() << "RTCSocket::Index[" << linkIndex << "]=" << message;
      } break;
    default:
      break;
    }
}

void RTCSocket::onReceivedGopPackage(QString message){
  QJsonDocument document = QJsonDocument::fromJson(message.toUtf8());
  QJsonObject jsonObject = document.object();
  QJsonValue gopJsonValue = jsonObject.take("gop");
  QJsonArray jsonArrayTimestamp = gopJsonValue.toArray();
  for (int index = 0; index < jsonArrayTimestamp.size(); ++index) {
      double timestampPackage  = jsonArrayTimestamp.at(index).toDouble();
      QString timestampString = QString::number(timestampPackage, 'g' , 13);
      QString downloadLink = linkFormat.arg(timestampString);
      pRTCDownloader->onReceiveDowloadRTCData(linkIndex++, downloadLink);
    }
}

void RTCSocket::prepareLinkToDownload(QString message) {
  QString mess = message.replace("[", "").replace("]", "");
  QStringList listValue = mess.split(",");
  QString timestamp = listValue.at(0);
  QString idrByte = listValue.at(3);
  int isIDRFrame = idrByte.toInt();
  if (!isSecondIdrReached) {
    if (isIDRFrame == 0) {
      return;
    } else {
      isSecondIdrReached = true;
    }
  }

  QString downloadLink = linkFormat.arg(timestamp);
  pRTCDownloader->onReceiveDowloadRTCData(linkIndex++, downloadLink);
}

void RTCSocket::closeSource() {
  QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  qDebug() << playerName << "::RTCSocket::closeSource" ;
  this->socketUrl = "";
  needToOpenSocketWhenOldSocketDisconnected = false;

  if (socket->isValid()) {
      socket->close();
    }
}

void RTCSocket::onSocketDisconnected() {
  QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  qDebug() << playerName << "::RTCSocket::onSocketDisconnected== NAM" ;
  socketState = RTCSS_Disconnected;
  if (needToOpenSocketWhenOldSocketDisconnected && isNetworkReachable) {
      needToOpenSocketWhenOldSocketDisconnected = false;
              openSocket();
    }
  //Q_EMIT socketDisconnected();
}

void RTCSocket::newAction(int message, QVariant *attachment) {
  Q_UNUSED(attachment)
  switch (message) {
    case Message.PLAYER_SOURCE_CLEAR: {
        closeSource();
      } break;

    case Message.APP_NETWORK_IS_REACHABLE: {
        isNetworkReachable = true;
        if (this->pRTCPlayer->isPlayerPlaying() && socketState == RTCSS_ReceivedDownloadLink) {
            openSocket();
          }
      } break;

    case Message.APP_NETWORK_IS_UNREACHABLE: {
        isNetworkReachable = false;
      } break;

    default:
      qDebug() << "ERROR : General Internal pac action in RTCPlayer "
               << "non-catched :" + Message.toString(message);
    }
}

