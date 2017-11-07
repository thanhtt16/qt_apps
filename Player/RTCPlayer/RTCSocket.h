#ifndef RTCSOCKET_H
#define RTCSOCKET_H

#include <QDateTime>
#include <QImage>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QStack>
#include <QHash>
#include <QVector>

#include <QtWebSockets/QWebSocket>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "RTCPlayer.h"
#include "RTCSplitter.h"
#include <QMutex>
#include <message.h>

class RTCPlayer;
class RTCDownloader;
class RTCSplitter;

enum RTCSocketState {
  RTCSS_Disconnected,
  RTCSS_Connected,
  RTCSS_SentSignature,
  RTCSS_ReceivedSignature,
  RTCSS_ReceivedInfomation,
  RTCSS_ReceivedDownloadLink
};

class RTCSocket : public QObject {
  Q_OBJECT

private:
    RTCPlayer *pRTCPlayer;
    AppMessage Message;
    bool playerIsPlaying = false;
    RTCDownloader *pRTCDownloader;

    QWebSocket *socket;
    RTCSocketState socketState = RTCSS_Disconnected;
    bool autoReconnect = true;
    QTimer *reconnectTimer = Q_NULLPTR;

    QString socketUrl = "";
    QString linkFormat = "";
    bool isAllowReceiving = true;
    bool isSourceChanged = false;
    bool needToOpenSocketWhenOldSocketDisconnected = false;
    bool playerIsPlayingAndNeedToReconnectWhenSocketDisconnectedByAccident = false;
    quint64 linkIndex = 0;
    bool isSecondIdrReached = false;

    bool isNetworkReachable = true;

    QString host;
    QString mac;
    QString cam;

    void setNewSource(QString source);
    void prepareLinkToDownload(QString message);
    void resetInternalState();

public:
    RTCSocket(RTCPlayer *rtcPlayer);
    ~RTCSocket();

    void openSource(QString source);
    void closeSource();

    void openSocket();
    void sendSignature();
    void newAction(int message, QVariant *attachment);

    RTCPlayer *getPRTCPlayer() const;
    void onReceivedGopPackage(QString message);

Q_SIGNALS:
    void socketConnected();
    void socketDisconnected();
    void downloadRTCData(quint64 linkIndex, QString url);

public Q_SLOTS:
    void onSocketConnected();
    void onSocketDisconnected();

    void onRawPackageIndexReceived(const QString &message);
};

#endif  // RTCSOCKET_H
