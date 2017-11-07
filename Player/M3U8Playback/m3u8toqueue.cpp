#include "m3u8toqueue.h"

M3U8ToQueue::M3U8ToQueue(QObject *parent) : QObject(parent)
{
    connect(&manager, &QNetworkAccessManager::finished, this, &M3U8ToQueue::onFetchFinished);
}

void M3U8ToQueue::setNewSource(QString m3u8Url) {
    if (isFetching) {
        qDebug() << QString("I'm fetching file m3u8: %1").arg(this->m3u8Url);
        return;
    }
    this->m3u8Url = m3u8Url;
    if (this->m3u8Url.contains(".m3u8")) {
        this->baseUrl = m3u8Url.replace(m3u8Url.split("/").last(), "");
        this->tsUrls.clear();
        this->startFetch(this->m3u8Url);
    } else {
        qDebug() << "Url" << this->m3u8Url << "is invalid!";
        this->m3u8Url = "";
    }
}

void M3U8ToQueue::startFetch(QString url) {
    if(!this->isFetching) {
        this->isFetching = true;
        manager.get(QNetworkRequest(QUrl(url)));
        qDebug() << QString("Started fetching file m3u8: %1").arg(this->m3u8Url);
    }
}

void M3U8ToQueue::onFetchFinished(QNetworkReply *reply) {
    QString data = QString(reply->readAll());
    qDebug() << QString("Finished fetching file m3u8: %1").arg(this->m3u8Url);
//    qDebug() << data;
    this->parseM3u8(data);
    reply->deleteLater();
    this->isFetching = false;
}

void M3U8ToQueue::enqueueTsUrl(QString tsUrl) {
    this->tsUrls.enqueue(tsUrl);
}

QString M3U8ToQueue::dequeueTsUrl() {
    if (this->tsUrls.isEmpty()) {
        return "";
    }
    return this->tsUrls.dequeue();
}

void M3U8ToQueue::parseM3u8(QString data) {
    if (data == "#No data") {
        qDebug() << "No Data";
    } else {
        QStringList lines = data.split("\n");
        for (int var = 0; var < lines.size(); ++var) {
            QString url = lines.at(var);
            if (url.contains(".h264")) {
                url = this->baseUrl + url;
                qDebug() << QString("%1 -> %2").arg(this->tsUrls.size(), 5).arg(url);
                this->enqueueTsUrl(url);
            }
        }
    }
    qDebug() << "Fetched ts files" << this->tsUrls.size();
}
