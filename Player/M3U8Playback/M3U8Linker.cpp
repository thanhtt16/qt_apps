#include "M3U8Linker.h"

M3U8Linker::M3U8Linker(PlaybackPlayer *rtcPlayer)
{
    this->rtcPlayer = rtcPlayer;

}

void M3U8Linker::run()
{
    M3U8ToQueue *m3u8Queue = this->getRtcPlayer()->getM3u8ToQueue();
    PlaybackDownloader *downloader = this->rtcPlayer->getDownloader();
    while (1) {
        QString tsFileUrl = m3u8Queue->dequeueTsUrl();
        Q_EMIT playBackDownload(dataIndex++, tsFileUrl);
        QThread::msleep(1000);
    }
}

void M3U8Linker::startWorking()
{
    if (!isRunning()) start();


}

void M3U8Linker::stopWorking()
{
    if (isRunning()) quit();
}

PlaybackPlayer *M3U8Linker::getRtcPlayer() const
{
    return rtcPlayer;
}
