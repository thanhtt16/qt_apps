#include "PlaybackSocket.h"

PlaybackSocket::PlaybackSocket(PlaybackPlayer *rtcPlayer) : pRTCPlayer(rtcPlayer) {
    socket = new QWebSocket();
    connect(socket, &QWebSocket::connected, this, &PlaybackSocket::onSocketConnected);
    connect(socket, &QWebSocket::disconnected, this, &PlaybackSocket::onSocketDisconnected);
    connect(socket, &QWebSocket::textMessageReceived, this, &PlaybackSocket::onRawPackageIndexReceived);

    //reconnectTimer = new QTimer(this);
    //connect(reconnectTimer, &QTimer::timeout, this, &RTCSocket::onReconnectSocket);

    pRTCDownloader = new PlaybackDownloader(pRTCPlayer);
}

PlaybackSocket::~PlaybackSocket() {
    socket->deleteLater();
}

PlaybackPlayer *PlaybackSocket::getPRTCPlayer() const
{
    return pRTCPlayer;
}

void PlaybackSocket::openSource(QString source) {
    if (socket->isValid() && (this->socketUrl == source)) return;

    setNewSource(source);

    if (socket->isValid()) {
        needToOpenSocketWhenOldSocketDisconnected = true;
        socket->close();
    } else {
        openSocket();
    }
}

void PlaybackSocket::setNewSource(QString source) {

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

void PlaybackSocket::makePlaybackSource(QString source) {
//    socketUrl = source;
//    //buildLinkFormat for download link
//    QString tmpUrl = socketUrl;
//    tmpUrl.replace("//", "/");
//    QStringList splitedUrl = tmpUrl.split("/");
//    if (splitedUrl.length() == 5) {
//        host = splitedUrl.at(1);
//        mac = splitedUrl.at(3);
//        cam = splitedUrl.at(4);
//    }
//    // playback
//    QString playBackSource = QString("http://%1/rec/hls/%2_%3_%4.m3u8").arg(host).arg(cam).arg("1506593410").arg("732000");
//    this->getPRTCPlayer()->getM3u8ToQueue().setNewSource(playBackSource);
}

void PlaybackSocket::openSocket() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSocket::openSocket" ;
    if (!socketUrl.isEmpty()) {
        if (!socket->isValid()) {
            playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = false;
            socket->open(QUrl(socketUrl));
        } else {
            needToOpenSocketWhenOldSocketDisconnected = true;
            socket->close();
        }
    }
}

void PlaybackSocket::onSocketConnected() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSocket::onSocketConnected" ;

    playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = true;
    sendSignature();

    Q_EMIT socketConnected();
}

void PlaybackSocket::sendSignature() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSocket::sendSignature" ;

    PlaybackMapQueue& rtpQueue = this->pRTCPlayer->getRtpQueue();
    rtpQueue.empty();

    playerIsPlaying = true;

    QString signature = "{ \"action\":\"hello\", "
                        "\"version\":\"2.0\", "
                        "\"host_id\":\"%1\", "
                        "\"signature\":\"RESERVED\", "
                        "\"timestamp\":\"%2\" }";
    signature = signature.arg(mac).arg(QDateTime::currentMSecsSinceEpoch());
    socket->sendTextMessage(signature);
    socketState = Playback_SS_SentSignature;
}

void PlaybackSocket::onRawPackageIndexReceived(const QString &message) {
        switch (socketState) {
        case Playback_SS_SentSignature: {
            socketState = Playback_SS_ReceivedSignature;
        } break;
        case Playback_SS_ReceivedSignature: {
            socketState = Playback_SS_ReceivedDownloadLink;
            prepareLinkToDownload(linkIndex++, message);
        } break;
        case Playback_SS_ReceivedDownloadLink: {
            prepareLinkToDownload(linkIndex++, message);
        } break;
        default:
            break;
        }
}

void PlaybackSocket::prepareLinkToDownload(quint64 linkIndex, QString message) {
    auto getTimeStamp = [](QString message) {
        QString timestamp = "";
        if (message.length() > 14) {
            timestamp = message.mid(1, 13);
        }
        return timestamp;
    };

    QString downloadLink = linkFormat.arg(getTimeStamp(message));
    pRTCDownloader->onReceiveDowloadRTCData(linkIndex, downloadLink);
}

void PlaybackSocket::closeSource() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSocket::closeSource" ;

    if (socket->isValid()) {
        this->socketUrl = "";
        socket->close();
        this->getPRTCPlayer()->getRtpQueue().empty(); // clear queue
    }
}

void PlaybackSocket::onSocketDisconnected() {
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName << "::RTCSocket::onSocketDisconnected== NAM" ;
    socketState = Playback_SS_Disconnected;

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

void PlaybackSocket::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.APP_NETWORK_IS_REACHABLE: {
    } break;

    case Message.APP_NETWORK_IS_UNREACHABLE: {
        this->pRTCDownloader->clear();
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in RTCPlayer "
                 << "non-catched :" + Message.toString(message);
    }
}

