#ifndef PLAYBACKRENDER_H
#define PLAYBACKRENDER_H

#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QImage>
#include <QPainter>
#include <QThread>
#include <QtCore>
#include "PlaybackPlayer.h"
#include "PlaybackRenderWidget.h"
#include "PlaybackImageQueue.h"

class PlaybackPlayer;
class PlaybackRenderWidget;

class PlaybackRender : public QThread {
    Q_OBJECT

private:
    PlaybackPlayer *pRTCPlayer;
    PlaybackRenderWidget *rendererWidget;
    int freeTime = 0;
    bool playerIsPlaying = false;
    QMutex PLAYER_STATUS;


public:
    explicit PlaybackRender(PlaybackPlayer *rtcPlayer);
    ~PlaybackRender();

    void startWorking();
    void stopWorking();

    PlaybackRenderWidget *getRendererWidget() const;
    void setRendererWidget(PlaybackRenderWidget *value);
    QImage decodeRTPPackage(QByteArray rtpPackage);
    void decoderClose();
    void decoderOpen();
Q_SIGNALS:
    void startPlaying();
    void startLoading();
    void startReplaying();

    void showPlayerLayer();
    void showLoadingSign();
    void showBlackLayer();

protected:
    void run();
};

#endif  // PLAYBACKRENDER_H
