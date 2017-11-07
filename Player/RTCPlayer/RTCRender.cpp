#include "RTCRender.h"

RTCRender::RTCRender(RTCPlayer *rtcPlayer) {
    this->pRTCPlayer = rtcPlayer;
    this->setObjectName("Render");
}

RTCRender::~RTCRender() {
}

void RTCRender::setRendererWidget(RTCRenderWidget *value) {
    rendererWidget = value;
}

RTCRenderWidget* RTCRender::getRendererWidget() const {
    return rendererWidget;
}

void RTCRender::startWorking() {
    qDebug() << "RTCRender::startWorking";
    Cam9ImageQueue& imageQueue = this->pRTCPlayer->getImageQueue();
    imageQueue.setWorking(true);
    imageQueue.empty();
    if (!isRunning()) {
        QThread::start();
    }
    isFirstShowNoCamera = false;
    renderMutex.lock();
    working = true;
    renderMutex.unlock();
}

void RTCRender::stopWorking() {
    qDebug() << "RTCRender::stopWorking" << pRTCPlayer->getPlayerName();
    Cam9ImageQueue& imageQueue = this->pRTCPlayer->getImageQueue();
    imageQueue.setWorking(false);
    imageQueue.empty();
    renderMutex.lock();
    working = false;
    renderMutex.unlock();
    isFirstShowNoCamera = false;
}

bool RTCRender::isWorking() {
    bool w;
    renderMutex.lock();
    w = working;
    renderMutex.unlock();
    return w;
}

void RTCRender::run() {
    int refreshRate = 40;
    long isShowingLoadingSign = 1;
    QString playerName = "Player " + this->pRTCPlayer->getPlayerName();
    qDebug() << playerName <<  "::RTCRender::RUNING ";
    Cam9ImageQueue& imageQueue = this->pRTCPlayer->getImageQueue();
    imageQueue.setName("Image Queue of " + playerName);
    while (1) {
        //no camera
        if(!isWorking() && !isFirstShowNoCamera){
            Q_EMIT showNoCameraLayer();
            isFirstShowNoCamera = true;
            QThread::msleep(1);// trong truong hop chay khong tai de cpu khong bi tang
        }
        while (isWorking()) {
            // reach the low water mark
            //        if (imageQueue.getSize() == 0) {
            //            isShowingLoadingSign = 1;
            //            Q_EMIT showLoadingSign();
            //            while (imageQueue.getSize() < fps) {
            //                qDebug() << "playerName" <<playerName << "SLEEP NOT HAS IMAGES";
            //                QThread::msleep(10);
            //              }
            //          }
            imageQueue.dequeue();
            QImage image = imageQueue.getDequeueImage();
            if (!image.isNull()) {
                if (this->missedImages > 0) {
                    this->missedImages = 0;
                    Q_EMIT showPlayerLayer();
                }
                this->drawedImages++;
                rendererWidget->onDrawImageFromRenderer(image);
                if (isShowingLoadingSign == 1) {
                    isShowingLoadingSign = 0;
                }
            }else {
                this->missedImages++;
                if (this->missedImages == this->fps) {
                    this->drawedImages = 0;
                    Q_EMIT showLoadingSign();
                }
                //        qDebug() << "* PlaybackRender -> missedImages:"
                //                 << this->missedImages;
            }

            int size = imageQueue.getSize();
            if ((size > 2*fps) || ((size >= fps ) && (refreshRate == 20))) {
                refreshRate = 20; // 5 times faster
            } else {
                refreshRate = 40;
            }
            QThread::msleep(refreshRate);
        }
        QThread::msleep(1);// trong truong hop chay khong tai de cpu khong bi tang
    }
    qDebug() << playerName << "::RTCRender::STOPPED ";
}

