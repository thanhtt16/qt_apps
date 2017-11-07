#ifndef RTCPLAYERWIDGET_H
#define RTCPLAYERWIDGET_H

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
#include <PacModel/control.h>

#include "RTCSplitter.h"
#include "RTCRender.h"
#include "RTCSocket.h"
#include "LinkManager.h"
#include "RTCDownloader.h"
#include "cam9mapqueue.h"
#include "message.h"
#include "Camera/camitem.h"
#include "NbtpSocket.h"

#include <QMutex>

class RTCSocket;
class LinkManager;
class RTCDownloader;
class RTCSplitter;
class RTCReceiver;
class RTCRender;
class RTCRenderWidget;
class cam9downloader;
class NbtpSocket;

enum RTCPlayerState {
  RTCPS_Loading,
  RTCPS_Playing,
  RTCPS_Paused,
  RTCPS_Stopped
};

class RTCPlayer : public QObject {
  Q_OBJECT

private:
    QWidget *zone = Q_NULLPTR;
    QWidget *topbarSpaceHolder;
    QWidget *videoZone;

    bool playerIsPlaying = false;

    AppMessage Message;
    QVBoxLayout *mainLayout;

    QNetworkAccessManager *naManager;
    RTCDownloader *rtpDownloader;

    // Streamer
    RTCSocket *pRTCSocket; // live stream manager :)

    // playback
    NbtpSocket *nbtpSocket;
    LinkManager *linkManager; // playback stream manager

    RTCSplitter *pRTCSplitter;
    RTCRender *pRTCRenderer;
    RTCRenderWidget *pRTCRendererWidget;

    Cam9MapQueue rtpQueue;
    Cam9ImageQueue imageQueue;
    QQueue<cam9downloader *> downloaderPool;
    QMutex downloaderPoolMutex;

    QString sourceUrl = "";
    bool isAutoPlay = true;
    bool isAutoReconnect = true;
    QString playerName;

    CamItem *camItem = Q_NULLPTR;
    CamItemType networkType;

    bool shouldPlayLive = true;

    bool useNbtpSocket = true;

public:
    RTCPlayer(QWidget *zone);
    ~RTCPlayer();

    //void rtcPlayerOpen(QString source);
    void startWorking();
    void stopWorking();

    bool getIsAutoReconnect();
    void setIsAutoReconnect(bool value);

    bool getIsAutoPlay() const;
    void setIsAutoPlay(bool value);
    QString getPlayerName() const { return playerName; }
    void setPlayerName(QString value) {
      playerName = value;
    }
    void wheelEventZoomVideo(QVariant *dataStruct);
    void eventMoveVideo(QVariant *dataStruct);

    Cam9MapQueue& getRtpQueue() { return rtpQueue; }
    Cam9ImageQueue& getImageQueue() { return imageQueue; }
    RTCSocket* getPRTCSocket()  { return pRTCSocket; }


    RTCSplitter *getPRTCSplitter() const;

    QString getSourceUrl() const;
    void newAction(int message, QVariant *attachment);
    void setSourceUrl(const QString &value);
    void playSource();
    void onDownloadNbtpLink(quint64 linkIndex, QString url);

    RTCDownloader *getRtpDownloader() const;

    bool getShouldPlayLive() const;

    QNetworkAccessManager *getNaManager() const;

    NbtpSocket *getNbtpSocket() const;

    bool isPlayerPlaying() const;
    void takeScreenshoot();
public Q_SLOTS:
    void onSocketConnected();
    void onSocketDisconnected();

    void onShowLoadingSign();
    void onShowPlayerLayer();
    void onShowNoCameraLayer();

    void onSetPlayerDisplayName(QString displayName) {
        this->playerName = displayName;
    }
protected:
  bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
    void showLoadingSign();
    void showPlayerLayer();
    void showNoCameraLayer();

};

#endif  // RTCPLAYERWIDGET_H
