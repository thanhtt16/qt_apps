#ifndef RTCRENDER_H
#define RTCRENDER_H

#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include <QThread>
#include <QtCore>
#include "RTCPlayer.h"
#include "RTCRenderWidget.h"
#include "cam9imagequeue.h"

class RTCPlayer;
class RTCRenderWidget;

class RTCRender : public QThread {
    Q_OBJECT

private:
    RTCPlayer *pRTCPlayer;
    RTCRenderWidget *rendererWidget;
    int fps = 25;
    int freeTime = 0;
    bool working = false;
    QMutex renderMutex;
    bool isFirstShowNoCamera = false;

public:
    explicit RTCRender(RTCPlayer *rtcPlayer);
    ~RTCRender();

    void startWorking();
    void stopWorking();
    bool isWorking();

    RTCRenderWidget *getRendererWidget() const;
    void setRendererWidget(RTCRenderWidget *value);
    long missedImages = 0;
    long drawedImages = 0;
Q_SIGNALS:
    void startPlaying();
    void startLoading();
    void startReplaying();

    void showPlayerLayer();
    void showLoadingSign();
    void showNoCameraLayer();

protected:
    void run();
};

#endif  // RTCRENDER_H
