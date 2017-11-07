#include "Camera/camstream.h"
#include "Common/logutils.h"

CamProfile *CamStream::getCamProfile() const { return camProfile; }

void CamStream::setCamProfile(CamProfile *value) { camProfile = value; }

void CamStream::setSource(const QString &value) { source = value; }

CamStream::CamStream(CamProfile *parent) : QObject(parent) {
    this->camProfile = camProfile;
}

CamStream *CamStream::parse(CamProfile *camProfile, QJsonObject jsonObject) {
    CamStream *camStream = new CamStream(camProfile);
    QJsonValue jsonValue;
    jsonValue = jsonObject.take("thumb");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camStream->thumb = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("source");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camStream->source = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("network");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camStream->network = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("protocol");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camStream->protocol = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("device_id");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camStream->deviceId = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("channel_id");
    if (!jsonValue.isNull()) {
        int value = jsonValue.toInt();
        camStream->channelId = value;
    }

    return camStream;
}

QString CamStream::getThumb() { return thumb; }

QString CamStream::getSource() { return source; }

QString CamStream::getNetwork() { return network; }

QString CamStream::getProtocol() { return protocol; }

QString CamStream::getDeviceId() { return deviceId; }

int CamStream::getChannelId() { return channelId; }

bool CamStream::isPlaying() { return playing; }

void CamStream::setPlaying(bool p) { playing = p; }

bool CamStream::isPlayable() { return playable; }

void CamStream::setPlayable(bool p) { playable = p; }

QString CamStream::getSourceDataMap() { return sourceDataMap; }

void CamStream::setSourceDataMap(QString source) { sourceDataMap = source; }

void CamStream::checkHasConnection(
        std::function<void(void)> onHasConnection,
        std::function<void(void)> onHasNoConnection) {
    if (!this->source.isEmpty()) {
        if (this->source.count(":") > 1) {
            checkSocketConnection(onHasConnection, onHasNoConnection);
        } else {
            if (this->source.contains("ws://")) {
                checkSocketConnection(onHasConnection, onHasNoConnection);
            } else {
                checkRequestConnection(onHasConnection, onHasNoConnection);
            }
        }
    } else {
        onHasNoConnection();
    }
}

void CamStream::checkRequestConnection(
        std::function<void(void)> onHasConnection,
        std::function<void(void)> onHasNoConnection) {
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                     SLOT(quit()));
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(this->source)));
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        if (reply) {
            reply->deleteLater();
        }
        onHasConnection();
    } else {
        if (reply) {
            reply->deleteLater();
        }
        onHasNoConnection();
    }
}

void CamStream::checkSocketPortConnection(
        std::function<void(void)> onHasConnection,
        std::function<void(void)> onHasNoConnection) {
    bool isconnected = false;
    QString url = this->source;
    QStringList listUrl = url.split("\//");
    QString urlNew = listUrl[1];
    QString uri = urlNew.split(":").first();
    QString host = urlNew.split(":").last();

    QTcpSocket *socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this,
            [socket, onHasConnection, &isconnected] {
        isconnected = true;
        onHasConnection();
        socket->close();
    });

    connect(socket, &QTcpSocket::disconnected, this,
            [socket, onHasNoConnection, &isconnected] {
        if (!isconnected) {
            onHasNoConnection();
            socket->close();
        } else {
            socket->deleteLater();
        }
    });

    socket->connectToHost(url, host.toInt());
    // we need to wait...
    if (!socket->waitForConnected(500)) {
        qDebug() << "Error: " << socket->errorString();
    }
}

void CamStream::checkSocketConnection(
        std::function<void(void)> onHasConnection,
        std::function<void(void)> onHasNoConnection) {
    QWebSocket *webSocket = new QWebSocket();
    isConnected = false;
    connect(webSocket, &QWebSocket::connected, this,
            [this, webSocket, onHasConnection] {
        isConnected = true;
        qDebug() << Q_FUNC_INFO << "CONNECTED";
        onHasConnection();
        webSocket->close();
        webSocket->deleteLater();
    });
    connect(webSocket, &QWebSocket::disconnected, this,
            [this, webSocket, onHasNoConnection] {
        if (!isConnected) {
            qDebug() << Q_FUNC_INFO << "DISCONNECTED";
            webSocket->close();
            webSocket->deleteLater();
            onHasNoConnection();
        }
    });

    webSocket->open(QUrl(this->source));
    QTimer::singleShot(500, [this, onHasNoConnection, webSocket] {
        if (!isConnected) {
            onHasNoConnection();
        } else {
        }
    });
}

