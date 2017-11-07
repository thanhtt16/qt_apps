#include "NbtpSocket.h"

NbtpSocket::NbtpSocket(RTCPlayer *rtcPlayer) : pRTCPlayer(rtcPlayer) {
    socket = new QWebSocket();
    connect(socket, &QWebSocket::connected, this, &NbtpSocket::onSocketConnected);
    connect(socket, &QWebSocket::disconnected, this, &NbtpSocket::onSocketDisconnected);
    connect(socket, &QWebSocket::binaryMessageReceived, this, &NbtpSocket::onBinaryMessageReceived);
}

NbtpSocket::~NbtpSocket() {
    socket->deleteLater();
}

RTCPlayer *NbtpSocket::getPRTCPlayer() const
{
    return pRTCPlayer;
}

void NbtpSocket::openSource(QString source) {
    if (socket->isValid() && (this->socketUrl == source)) return;

    setNewSource(source);

    if (socket->isValid()) {
        needToOpenSocketWhenOldSocketDisconnected = true;
        socket->close();
    } else {
        openSocket();
    }
}

void NbtpSocket::setNewSource(QString source) {

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

    //ws://${dst_hostname}/nbtp/${this_host_id}/${chn.name}
    nbtpSocketLink = QString("ws://%1/nbtp/%2/%3").arg(host).arg(mac).arg(cam);
}


void NbtpSocket::openSocket() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::NbtpSocket::openSocket" ;
    if (!socketUrl.isEmpty()) {
        if (!socket->isValid()) {
            playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = false;
            socket->open(QUrl(nbtpSocketLink));
            //qDebug() << nbtpSocketLink;
        } else {
            needToOpenSocketWhenOldSocketDisconnected = true;
            socket->close();
        }
    }
}

void NbtpSocket::onSocketConnected() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::NbtpSocket::onSocketConnected" ;

    playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = true;
    sendSignature();

    Q_EMIT socketConnected();
}

void NbtpSocket::sendSignature() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::NbtpSocket::sendSignature" ;

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
    socketState = NBTPSS_SentSignature;
}

void NbtpSocket::onBinaryFrameReceived(const QByteArray &frame, bool isLastFrame) {
    qDebug() << "Last Frame: " << isLastFrame << " -size = " << frame.size();
}

void NbtpSocket::onBinaryMessageReceived(const QByteArray &message) {
    qDebug() << "package: " << " size = " << message.size();
    qDebug() << message;
    socket->sendBinaryMessage(packRequest());
}

QByteArray NbtpSocket::packRequest() {
    quint32 CONSUMER_REQ_SEG = 0;
      quint32 div0sec = 1507111800000;
      quint16 begin_pkt_id = 1;
      quint16 begin_dts = 0;
      quint32 end_div0sec = 1507113600000;
      quint16 end_pkt_id = 1200;
      quint16 end_dts = 600000;
      double req_id = 1.0;

      QByteArray req;
      QDataStream ds(&req, QIODevice::WriteOnly);
      ds << CONSUMER_REQ_SEG;
      ds << div0sec;
      ds << begin_pkt_id;
      ds << begin_dts;
      ds << end_div0sec;
      ds << end_pkt_id;
      ds << end_dts;
      ds << req_id;

    return req;
}

void NbtpSocket::closeSource() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::NbtpSocket::closeSource" ;

    if (socket->isValid()) {
        this->socketUrl = "";
        playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = false;
        needToOpenSocketWhenOldSocketDisconnected = false;
        socket->close();
        this->getPRTCPlayer()->getRtpQueue().empty(); // clear queue
        this->getPRTCPlayer()->getImageQueue().empty();
    }
}

void NbtpSocket::onSocketDisconnected() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::NbtpSocket::onSocketDisconnected== NAM" ;
    socketState = NBTPSS_Disconnected;

    this->getPRTCPlayer()->getRtpQueue().empty(); // clear queue

    if (needToOpenSocketWhenOldSocketDisconnected) {
        needToOpenSocketWhenOldSocketDisconnected = false;
        openSocket();
    }
//        if (networkIsReachable) {
//            shouldOpenSocketWhenNetworkBecomeReachable = false;
//            openSocket();
//        }
//        else {
//            shouldOpenSocketWhenNetworkBecomeReachable = true;
//        }

    //Q_EMIT socketDisconnected();
}

void NbtpSocket::newAction(int message, QVariant *attachment) {
  Q_UNUSED(attachment)
    switch (message) {
    case Message.APP_NETWORK_IS_REACHABLE: {
    } break;

    case Message.APP_NETWORK_IS_UNREACHABLE: {
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in NbtpSocket "
                 << "non-catched :" + Message.toString(message);
    }
}

