#ifndef PLAYBACKSOCKET_H
#define PLAYBACKSOCKET_H

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
#include "M3U8Linker.h"
#include "PlaybackPlayer.h"
#include "PlaybackSplitter.h"
#include <QMutex>
#include <message.h>

class PlaybackPlayer;
class PlaybackDownloader;
class PlaybackSplitter;

enum PlaybackSocketState {
  Playback_SS_Disconnected,
  Playback_SS_Connected,
  Playback_SS_SentSignature,
  Playback_SS_ReceivedSignature,
  Playback_SS_ReceivedInfomation,
  Playback_SS_ReceivedDownloadLink
};

class PlaybackSocket : public QObject {
  Q_OBJECT

private:
    PlaybackPlayer *pRTCPlayer;
    AppMessage Message;
    bool playerIsPlaying = false;
//    QMutex PLAYER_STATUS;
    PlaybackDownloader *pRTCDownloader;

    QWebSocket *socket;
    PlaybackSocketState socketState = Playback_SS_Disconnected;
    bool autoReconnect = true;
    QTimer *reconnectTimer = Q_NULLPTR;

    QString socketUrl = "";
    QString linkFormat = "";
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
    void resetInternalState();

    // playback
    void makePlaybackSource(QString source);

public:
    PlaybackSocket(PlaybackPlayer *rtcPlayer);
    ~PlaybackSocket();

    void openSource(QString source);
    void closeSource();

    void openSocket();
    void sendSignature();
    void newAction(int message, QVariant *attachment);

    PlaybackPlayer *getPRTCPlayer() const;


Q_SIGNALS:
    void socketConnected();
    void socketDisconnected();
    void downloadRTCData(quint64 linkIndex, QString url);

public Q_SLOTS:
    void onSocketConnected();
    void onSocketDisconnected();

    void onRawPackageIndexReceived(const QString &message);
};

#endif  // PLAYBACKSOCKET_H
