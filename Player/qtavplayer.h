#ifndef QTAVPLAYER_H
#define QTAVPLAYER_H
#include <QWidget>

#include <QVBoxLayout>

#include <QHBoxLayout>

#include <QLabel>

#include <QtAVWidgets>

#include <QtAV>

#include <QStackedLayout>

#include <functional>

#include <QGraphicsDropShadowEffect>

#include <QPushButton>


#include "Camera/camitem.h"

#include "Camera/camstream.h"

#include "Camera/camstatistic.h"


#include "Common/appstat.h"

#include "Common/networkutils.h"

#include "Common/resources.h"


using namespace QtAV;

using namespace std;


static struct VideoCellConfigs {



    const int statusWidth = 40;

    const int statusHeight = 20;

    const int statusRadius = 4;



    const int spacing = 5;

    const int margin = 5;



    //        const int slowFps = 8;

    //        const int stopFps = 4;



} videoCellConfigs;


enum VideoCellState {

    VCSLoad,

    VCSPlay,

    VCSPause,

    VCSStop

};


class QTAVPlayer : public QWidget

{

    Q_OBJECT

private:

    VideoCellState mState = VCSLoad;

    QString mVodUrl = "";

    QStackedLayout *mVideoCellLayout = NULL;

    QVBoxLayout *pVideoLayout = NULL;

    QHBoxLayout *mTextLayout = NULL;

    QWidget *mVideoRendererWidget = NULL;

    AVPlayer *mAVPlayer = NULL;

    VideoRenderer *mVideoRenderer = NULL;

    void initPlayer();
    QSize mVideoSize;




//    void changeCamState(CamState);



    void onPlayerStartPlaying();

    void onPlayerPlaying();



    void onPlayerStartStopping();

    void onPlayerStopping();



    void onPlayerStopped();

    void onPlayerWaiting();




public:

    QTAVPlayer(QWidget *parent, QSize videoSize);


    void playVOD(QString, bool);

    void stop();

    void play();

    void pause();

    void replay();

    void release();


    void showVideo();

    void showMessage();

    VideoCellState getState() const;

protected:

    void resizeEvent(QResizeEvent*);

Q_SIGNALS:



public Q_SLOTS:
    void onChangeClockType();

    void onHandleError(const QtAV::AVError &error);

};

#endif // QTAVPLAYER_H
