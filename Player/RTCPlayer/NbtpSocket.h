#ifndef NBTPSOCKET_H
#define NBTPSOCKET_H

#include <QDateTime>
#include <QImage>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QStack>
#include <QHash>
#include <QVector>
#include <QDataStream>
#include <QtWebSockets/QWebSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "RTCPlayer.h"
#include "RTCSplitter.h"
#include <QMutex>
#include <message.h>

class RTCPlayer;
class RTCSplitter;

enum NBTPSocketState {
  NBTPSS_Disconnected,
  NBTPSS_Connected,
  NBTPSS_SentSignature,
  NBTPSS_ReceivedSignature,
  NBTPSS_ReceivedInfomation,
  NBTPSS_ReceivedDownloadLink
};

class NbtpSocket : public QObject {
  Q_OBJECT

private:
    RTCPlayer *pRTCPlayer;
    AppMessage Message;
    bool playerIsPlaying = false;

    QWebSocket *socket;
    NBTPSocketState socketState = NBTPSS_Disconnected;
    bool autoReconnect = true;
    QTimer *reconnectTimer = Q_NULLPTR;

    QString socketUrl = "";
    QString linkFormat = "";
    QString nbtpSocketLink = "";

    bool isAllowReceiving = true;
    bool isSourceChanged = false;
    bool needToOpenSocketWhenOldSocketDisconnected = false;
    bool playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = false;
    quint64 linkIndex = 0;

    QString host;
    QString mac;
    QString cam;

    void setNewSource(QString source);
    void prepareLinkToDownload(quint64 linkIndex, QString message);
    QByteArray packRequest();

public:
    NbtpSocket(RTCPlayer *rtcPlayer);
    ~NbtpSocket();

    void openSource(QString source);
    void closeSource();

    void openSocket();
    void sendSignature();
    void newAction(int message, QVariant *attachment);

    RTCPlayer *getPRTCPlayer() const;


Q_SIGNALS:
    void socketConnected();
    void socketDisconnected();
    void downloadRTCData(quint64 linkIndex, QString url);

public Q_SLOTS:
    void onSocketConnected();
    void onSocketDisconnected();
    void onBinaryFrameReceived(const QByteArray &frame, bool isLastFrame);
    void onBinaryMessageReceived(const QByteArray &message);

};

#endif  // RTCSOCKET_H
