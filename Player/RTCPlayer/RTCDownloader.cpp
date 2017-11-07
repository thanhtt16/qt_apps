#include "RTCDownloader.h"

RTCDownloader::RTCDownloader(RTCPlayer *rtcPlayer, QNetworkAccessManager *manager)
{
  pRTCPlayer = rtcPlayer;
  naManager = manager;
  connect(naManager, &QNetworkAccessManager::finished, this, &RTCDownloader::onDownloadFinished);
}

RTCDownloader::~RTCDownloader()
{
}

void RTCDownloader::onReceiveDowloadRTCData(quint64 linkIndex, QString url)
{
  QNetworkRequest req(url);
  QNetworkReply *reply = naManager->get(req);
  replyToIndex[reply] = linkIndex;
  lastLinkIndex = linkIndex;
}

void RTCDownloader::download_progress(qint64, qint64) {
  //    quint64 linkIndex = replyToIndex.take(reply);
  //    if (reply->bytesAvailable() > 0) {
  //        bool ret;
  //       // read header 12 bytes
  //       QByteArray headerBytes = reply->read(12);
  //       int payloadSize = headerBytes.mid(0, 2).toHex().toInt(&ret,16);
  //       double timestamp =  headerBytes.mid(2, 8).toDouble(&ret);
  //       int index = headerBytes.mid(10, 2).toHex().toInt(&ret,16);

  //       // read 1 rtp package
  //       int lengthOfH264RawData = payloadSize - 10;
  //       QByteArray h264Raw = reply->read(lengthOfH264RawData);
  //       onReceiveDownloadedData(index, headerBytes + h264Raw);
  //    }
}

void RTCDownloader::onDownloadFinished(QNetworkReply *reply) {
  quint64 linkIndex = replyToIndex.take(reply);

  if (linkIndex > lastLinkIndex) {
      // Bo goi download ve cham cua kenh truoc
      // Do linkIndex da bi reset ve 0 nen cac goi cua kenh truoc se co index greater
      qDebug() << "Nhieu.....................................................";
      return;
    }

  if (reply->bytesAvailable() > 0) {
      onReceiveDownloadedData(linkIndex, reply->readAll());
    } else {
      onReceiveDownloadDataFailed(linkIndex, reply->errorString());
    }
  reply->deleteLater();
}

void RTCDownloader::resetInternalState() {
  replyToIndex.clear();
}

void RTCDownloader::onReceiveDownloadedData(quint64 linkIndex, QByteArray payload) {

  Cam9MapQueue& rtpQueue =this->pRTCPlayer->getRtpQueue();
  rtpQueue.enqueue(linkIndex, payload);
  // qDebug() << linkIndex << "=====" << payload.size();
}

void RTCDownloader::onReceiveDownloadDataFailed(quint64 linkIndex, QString errorString) {
  QByteArray downloadFaildMessage;
  downloadFaildMessage.append((char)0);

  Cam9MapQueue& rtpQueue =this->pRTCPlayer->getRtpQueue();
  rtpQueue.enqueue(linkIndex, downloadFaildMessage);

  //    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
  //    qDebug() << playerName << " Thread: " << this->thread()
  //             << "RTCDownloader onReceiveDownloadDataFailed - linkIndex: " << linkIndex
  //             << " error: "  << errorString
  //             << " networkAccessible: " << naManager->networkAccessible();
}
