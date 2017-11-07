#include "LinkManager.h"

LinkManager::LinkManager(RTCPlayer *rtcPlayer) {
    this->rtcPlayer = rtcPlayer;
    this->downloader = this->rtcPlayer->getRtpDownloader();
    connect(this, &LinkManager::RtpDownload, this->downloader, &RTCDownloader::onReceiveDowloadRTCData);
}

void LinkManager::run()
{
    while (1) {
            QString rtpLink = m3u8Queue.dequeue();
            if (!rtpLink.isNull() && !rtpLink.isEmpty()) {
                Q_EMIT RtpDownload(linkIndex++, rtpLink);
            }
        QThread::msleep(2000);
    }
}

void LinkManager::openSource(QString newSource) {
    QString host, mac, cam;
    QString tmpUrl = newSource;
    tmpUrl.replace("//", "/");
    QStringList splitedUrl = tmpUrl.split("/");
    if (splitedUrl.length() == 5) {
        host = splitedUrl.at(1);
        mac = splitedUrl.at(3);
        cam = splitedUrl.at(4);
    }
    QString playBackSource = QString("http://%1/rec/hls/%2_%3_%4_nbtp.m3u8").arg(host).arg(cam).arg("1506848400000").arg("13197000");
    //playBackSource = "https://cdn.sandbox.cam9.tv/rec/hls/005a204c7a62xyz14689_1506848400000_3197000_nbtp.m3u8";
    qDebug() << playBackSource;
    this->m3u8Queue.setNewSource(playBackSource);
}


void LinkManager::startWorking()
{
    if (!isRunning()) start();
}

void LinkManager::stopWorking()
{
    if (isRunning()) quit();
}
