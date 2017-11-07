#include "PlaybackDownloader.h"

PlaybackDownloader::PlaybackDownloader(PlaybackPlayer *rtcPlayer):pRTCPlayer(rtcPlayer)
{
    naManager = new QNetworkAccessManager(this);
    connect(naManager, &QNetworkAccessManager::finished, this, &PlaybackDownloader::onDownloadFinished);
}

PlaybackDownloader::~PlaybackDownloader()
{
}

void PlaybackDownloader::onReceiveDowloadRTCData(quint64 linkIndex, QString url)
{
    QNetworkRequest req(url);
    QNetworkReply *reply = naManager->get(req);
    replyToIndex[reply] = linkIndex;

    PlaybackMapQueue& rtpQueue =this->pRTCPlayer->getRtpQueue();
    rtpQueue.updateLastLinkQueue(linkIndex);
}

void PlaybackDownloader::clear() {
}

void PlaybackDownloader::onDownloadFinished(QNetworkReply *reply) {
    quint64 linkIndex = replyToIndex.take(reply);
    if (reply->bytesAvailable() > 0) {
        onReceiveDownloadedData(linkIndex, reply->readAll());
    } else {
        onReceiveDownloadDataFailed(linkIndex, reply->errorString());
    }
    reply->deleteLater();
}

void PlaybackDownloader::resetInternalState() {
    replyToIndex.clear();
}

void PlaybackDownloader::onReceiveDownloadedData(quint64 linkIndex, QByteArray payload) {

    PlaybackMapQueue& rtpQueue =this->pRTCPlayer->getRtpQueue();
    rtpQueue.enqueue(linkIndex, payload);
}

void PlaybackDownloader::onReceiveDownloadDataFailed(quint64 linkIndex, QString errorString) {
//    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
//    qDebug() << playerName << " Thread: " << this->thread()
//             << "RTCDownloader onReceiveDownloadDataFailed - linkIndex: " << linkIndex
//             << " error: "  << errorString
//             << " networkAccessible: " << naManager->networkAccessible();
}
