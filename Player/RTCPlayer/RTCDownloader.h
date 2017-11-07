#ifndef RTCDOWNLOADER_H
#define RTCDOWNLOADER_H

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "RTCSocket.h"

class RTCSocket;
class RTCLinkManager;
class RTCDownloader: public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *naManager;
    quint64 lastLinkIndex = 0;
    QHash<QNetworkReply*, quint64> replyToIndex;
    RTCPlayer *pRTCPlayer;
    void onReceiveDownloadDataFailed(quint64 linkIndex, QString errorString);
    void onReceiveDownloadedData(quint64 linkIndex, QByteArray payload);
public:
    RTCDownloader(RTCPlayer *rtcPlayer, QNetworkAccessManager *naManager);
    ~RTCDownloader();

    void resetInternalState();
    void onReceiveDowloadRTCData(quint64 linkIndex, QString url);

public Q_SLOTS:
    void download_progress(qint64,qint64);
    void onDownloadFinished(QNetworkReply *reply);
};

#endif // RTCDOWNLOADER_H
