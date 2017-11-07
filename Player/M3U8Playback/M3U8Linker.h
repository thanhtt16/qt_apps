#ifndef M3U8Linker_H
#define M3U8Linker_H

#include <QThread>
#include <QNetworkAccessManager>
#include "PlaybackPlayer.h"
#include "m3u8toqueue.h"
#include "PlaybackDownloader.h"

class PlaybackPlayer;

class M3U8Linker : public QThread {
  Q_OBJECT
private:
    PlaybackPlayer *rtcPlayer;
    long dataIndex = 0;

    QNetworkAccessManager *networkManager;

public:
  explicit M3U8Linker(PlaybackPlayer *rtcPlayer);
    PlaybackPlayer *getRtcPlayer() const;
    void run();
    void onDownloadFinished(QNetworkReply *reply);
    void startWorking();
    void stopWorking();

Q_SIGNALS:
    void playBackDownload(quint64 linkIndex, QString url);
};

#endif  // M3U8Linker_H
