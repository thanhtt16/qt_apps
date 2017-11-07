#ifndef PLAYBACKPLAYER_H
#define PLAYBACKPLAYER_H

#include <QBuffer>
#include <QByteArray>
#include <QElapsedTimer>
#include <QFile>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QQueue>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "M3U8Linker.h"
#include "PlaybackSplitter.h"
#include "PlaybackRender.h"
#include "PlaybackSocket.h"
#include "PlaybackDownloader.h"
#include "PlaybackImageQueue.h"
#include "message.h"
#include "Camera/camitem.h"
#include "M3U8Linker.h"
#include "PlaybackRenderWidget.h"

#include <QMutex>

class PlaybackSocket;
class PlaybackDownloader;
class PlaybackSplitter;
class PlaybackRender;
class PlaybackRenderWidget;
class M3U8ToQueue;
class M3U8Linker;
class PlaybackMapQueue;
class PlaybackImageQueue;

enum PlaybackPlayerState {
  Playback_PS_Loading,
  Playback_PS_Playing,
  Playback_PS_Paused,
  Playback_PS_Stopped
};

class PlaybackPlayer : public QWidget {
  Q_OBJECT

private:
    //QMutex PLAYER_STATUS;
    AppMessage Message;
    QVBoxLayout *mainLayout;

    M3U8ToQueue *m3u8ToQueue;
    M3U8Linker *pM3U8Downloader;

    PlaybackDownloader *downloader;

    PlaybackSocket *pRTCSocket;
    PlaybackSplitter *pRTCSplitter;
    PlaybackRender *pRTCRenderer;
    PlaybackRenderWidget *pRTCRendererWidget;

    PlaybackMapQueue rtpQueue;
    PlaybackImageQueue imageQueue;

    QString sourceUrl = "";
    bool isAutoPlay = true;
    bool isAutoReconnect = true;
    QString playerName;

    CamItem *camItem = Q_NULLPTR;
    CamItemType type;

public:
    PlaybackPlayer(QWidget *parent = 0);
    ~PlaybackPlayer();

    //void rtcPlayerOpen(QString source);
    void rtcPlayerPlay();
    void rtcPlayerPause();
    void rtcPlayerStart();
    void rtcPlayerStop();

    bool getIsAutoReconnect();
    void setIsAutoReconnect(bool value);

    bool getIsAutoPlay() const;
    void setIsAutoPlay(bool value);
    QString getPlayerName() const { return playerName; }
    void setPlayerName(QString value) { playerName = value; }
    void zoomVideo(QVariant *dataStruct);
    void eventMoveVideo(QVariant *dataStruct);

    PlaybackMapQueue& getRtpQueue() { return rtpQueue; }
    PlaybackImageQueue& getImageQueue() { return imageQueue; }
    PlaybackSocket* getPRTCSocket()  { return pRTCSocket; }

    void resetQueues();


    PlaybackSplitter *getPRTCSplitter() const;

    QString getSourceUrl() const;
    void newAction(int message, QVariant *attachment);
    void setSourceUrl(const QString &value);
    void playSource();

    M3U8ToQueue *getM3u8ToQueue() const;
     void onReceiveDowloadRTCData(quint64 linkIndex, QString url);
    PlaybackDownloader *getDownloader() const;

public Q_SLOTS:
    void onSocketConnected();
    void onSocketDisconnected();

    void onShowLoadingSign();
    void onShowPlayerLayer();
    void onShowWallLayer();

    void onSetPlayerDisplayName(QString displayName) { this->playerName = displayName; }
    void makePlaybackSource(QString source);

Q_SIGNALS:
    void showLoadingSign();
    void showPlayerLayer();
    void showWallLayer();

};

#endif  // PLAYBACKPLAYER_H
