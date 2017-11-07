#include "cam9m3u8queue.h"

Cam9M3u8Queue::Cam9M3u8Queue() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Cam9M3u8Queue::onFetchFinished);
}

void Cam9M3u8Queue::setNewSource(QString m3u8Url) {
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

void Cam9M3u8Queue::startFetch(QString url) {
    if(!this->isFetching) {
        this->isFetching = true;
        manager.get(QNetworkRequest(QUrl(url)));
        qDebug() << QString("Started fetching file m3u8: %1").arg(this->m3u8Url);
    }
}

void Cam9M3u8Queue::onFetchFinished(QNetworkReply *reply) {
    QString data = QString(reply->readAll());
    qDebug() << QString("Finished fetching file m3u8: %1").arg(this->m3u8Url);
    this->parseM3u8(data);
    reply->deleteLater();
    this->isFetching = false;
}

void Cam9M3u8Queue::enqueue(QString tsUrl) {
    queueMutex.lock();
    //qDebug() << "Cam9M3u8Queue::dequeue::["<< this->tsUrls.size() <<"]" << tsUrl;
    this->tsUrls.enqueue(tsUrl);
    queueMutex.unlock();
}

QString Cam9M3u8Queue::dequeue() {
    QString url;
    queueMutex.lock();
    if (!this->tsUrls.isEmpty()) {
        url = this->tsUrls.dequeue();
        qDebug() << "Cam9M3u8Queue::dequeue::["<< tsUrls.size() <<"]" << url;
    }
    queueMutex.unlock();
    return url;
}

void Cam9M3u8Queue::parseM3u8(QString data) {
    if (data == "#No data") {
        qDebug() << "No Data";
    } else {
        QStringList lines = data.split("\n");
        for (int var = 0; var < lines.size(); ++var) {
            QString url = lines.at(var);
            if (url.contains(".nbtp")) {
                url = this->baseUrl + url;
                //qDebug() << QString("%1 -> %2").arg(this->tsUrls.size(), 5).arg(url);
                this->enqueue(url);
            }
        }
    }
    qDebug() << "Fetched ts files" << this->tsUrls.size();
}
