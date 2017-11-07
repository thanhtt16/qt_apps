#ifndef PLAYBACKDOWNLOADER_H
#define PLAYBACKDOWNLOADER_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "PlaybackSocket.h"
#include "PlaybackPlayer.h"

class PlaybackSocket;
class PlaybackPlayer;

class PlaybackDownloader: public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *naManager;
    PlaybackPlayer *pRTCPlayer;
    QHash<QNetworkReply*, quint64> replyToIndex;

    void onReceiveDownloadDataFailed(quint64 linkIndex, QString errorString);
    void onReceiveDownloadedData(quint64 linkIndex, QByteArray payload);
public:
    PlaybackDownloader(PlaybackPlayer *rtcPlayer);
    ~PlaybackDownloader();

    void resetInternalState();
    void onReceiveDowloadRTCData(quint64 linkIndex, QString url);
    void onDownloadFinished(QNetworkReply *reply);
    void clear();
};

#endif // PLAYBACKDOWNLOADER_H
