#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include <QThread>
#include <QNetworkAccessManager>
#include "RTCPlayer.h"
#include "RTCDownloader.h"
#include "cam9m3u8queue.h"

class RTCPlayer;
class RTCDownloader;
class Cam9M3u8Queue;

class LinkManager : public QThread {
  Q_OBJECT
private:
    quint64 linkIndex = 0;
    RTCPlayer *rtcPlayer;
    Cam9M3u8Queue m3u8Queue;
    RTCDownloader *downloader;
    QNetworkAccessManager *networkManager;

public:
  explicit LinkManager(RTCPlayer *rtcPlayer);
    void run();
    void startWorking();
    void stopWorking();
    void openSource(QString newSource);

Q_SIGNALS:
    void RtpDownload(quint64 linkIndex, QString url);
};

#endif  // M3U8Linker_H
