#include "PlaybackRender.h"

PlaybackRender::PlaybackRender(PlaybackPlayer *rtcPlayer) {
    this->pRTCPlayer = rtcPlayer;
    this->setObjectName("Render");
}

PlaybackRender::~PlaybackRender() {
}

void PlaybackRender::setRendererWidget(PlaybackRenderWidget *value) {
    rendererWidget = value;
}

PlaybackRenderWidget* PlaybackRender::getRendererWidget() const {
    return rendererWidget;
}

void PlaybackRender::startWorking() {
    if (!isRunning()) {
        QThread::start();
    }
}

void PlaybackRender::stopWorking() {
    qDebug() << "RTCRender::stopWorking";
}

void PlaybackRender::run() {
    static int fps = 25; // frames per second
    static int renderCost = 0; // miliseconds
    int refreshRate = 40;
    bool gopStarted = false;
    QImage image;
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    PlaybackImageQueue& imageQueue = this->pRTCPlayer->getImageQueue();
    imageQueue.setName("Image Queue of " + playerName);

    qDebug() << playerName <<  "::RTCRender::RUNING ";

    while (1) {
        QImage image = imageQueue.dequeue(0);
        if (!image.isNull()) {
            if (freeTime) {
                freeTime = 0;
                 Q_EMIT showPlayerLayer();
            }
            rendererWidget->onDrawImageFromRenderer(image);
        } else {
            freeTime++;
            if (freeTime == fps) {
                freeTime=0;
                Q_EMIT showLoadingSign();
            }
        }

        int size = imageQueue.getSize();
        if (size > 5*fps) {
            refreshRate = 8;
        } else {
            if (size < 1*fps) {
                refreshRate = 40;
            }
            else {
                refreshRate = 8;
            }
        }

        QThread::msleep(refreshRate);
    }
    qDebug() << playerName << "::RTCRender::STOPPED ";
}
