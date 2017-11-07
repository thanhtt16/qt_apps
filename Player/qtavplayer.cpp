#include "qtavplayer.h"

QTAVPlayer::QTAVPlayer(QWidget *parent, QSize videoSize) : QWidget(parent)

{
    Widgets::registerRenderers();
    setAttribute(Qt::WA_DeleteOnClose, true);

    mVideoCellLayout = new QStackedLayout();
    initPlayer();

    mVideoRenderer = VideoRenderer::create(VideoRendererId_OpenGLWidget);

    mVideoRenderer->setOutAspectRatioMode(VideoRenderer::OutAspectRatioMode::RendererAspectRatio);

    mVideoRenderer->setQuality(VideoRenderer::QualityBest);

    //mVideoRenderer->setPreferredPixelFormat(VideoFormat::Format_YUV420P);

    //mVideoRenderer->forcePreferredPixelFormat(true);

    mAVPlayer->setRenderer(mVideoRenderer);
    //renderwidget
    pVideoLayout = new QVBoxLayout();
    mVideoRendererWidget = mVideoRenderer->widget();

    mVideoRendererWidget->raise();

    mVideoRendererWidget->resize(mVideoSize);

    mVideoRendererWidget->setWindowFlags(mVideoRendererWidget->windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);

    mVideoRendererWidget->setWindowState(Qt::WindowNoState);

    mVideoRendererWidget->setAttribute(Qt::WA_DeleteOnClose);

    mVideoRendererWidget->setLayout(pVideoLayout);

    mVideoCellLayout->addWidget(mVideoRendererWidget);

    mVideoCellLayout->setCurrentWidget(mVideoRendererWidget);
}



void QTAVPlayer::showVideo() {

    mVideoCellLayout->setCurrentWidget(mVideoRendererWidget);

}




void QTAVPlayer::initPlayer() {

    mAVPlayer = new AVPlayer(this);

    mAVPlayer->setMediaEndAction(MediaEndAction_KeepDisplay);

    mAVPlayer->setBufferMode(BufferPackets);

    mAVPlayer->setBufferValue(1);

    //mAVPlayer->setInterruptOnTimeout(true);

    //mAVPlayer->setInterruptTimeout(10000);

    //mAVPlayer->setAsyncLoad(true);

    //mAVPlayer->setAutoLoad(true);

    //mAVPlayer->setAudioStream(0);

    //mAVPlayer->masterClock()->setClockAuto(false);

    //mAVPlayer->masterClock()->setClockType(AVClock::ExternalClock);

    //mAVPlayer->masterClock()->setInitialValue(QDateTime::currentSecsSinceEpoch());

    connect(mAVPlayer, &AVPlayer::error, this, &QTAVPlayer::onHandleError);

    //connect(mAVPlayer, &AVPlayer::started, this, &QTAVPlayer::onChangeClockType);

    QVariantHash optionForFormat;

    //    optionForFormat["flags"] = "nobuffer";

    optionForFormat["flags"] = "low_delay";

    optionForFormat["me_method"] = "zero";

    optionForFormat["tune"] = "zerolatency,fastdecode";

    optionForFormat["probesize"] = 10 * 1024 * 1024;

    optionForFormat["analyzeduration"] = 10 * 1000 * 1000;

    mAVPlayer->setOptionsForFormat(optionForFormat);

}


void QTAVPlayer::onChangeClockType() {

    AVPlayer* player = qobject_cast<AVPlayer*>(sender());

    player->masterClock()->setClockAuto(false);

    player->masterClock()->setClockType(AVClock::ExternalClock);

}


void QTAVPlayer::onHandleError(const AVError &error) {

    if (error.ffmpegErrorCode() == -1482175992) {



        release();

    } else {



    }

}




//void QTAVPlayer::changeCamState(CamState camState) {

//    mCamState = camState;

//    switch (mCamState) {

//    case CamStateLoading:

//        mStatusLabel->setFixedSize(10, 10);

//        mStatusLabel->setStyleSheet(utils.borderStyleSheet(colors.LOADING, colors.WHITE, 0, 5));

//        mStatusLabel->setText("");

//        break;

//    case CamStatePlaying:

//        mStatusLabel->setFixedSize(40, 20);

//        mStatusLabel->setStyleSheet(utils.borderStyleSheet(mIsLive ? colors.LIVE : colors.VOD, colors.WHITE, 0, 5));

//        mStatusLabel->setText(mIsLive ? "LIVE" : "VOD");

//        break;

//    case CamStateStopping:

//        mStatusLabel->setFixedSize(10, 10);

//        mStatusLabel->setStyleSheet(utils.borderStyleSheet(colors.PLAYING, colors.WHITE, 0, 5));

//        mStatusLabel->setText("");

//        break;

//    case CamStateStopped:

//        mStatusLabel->setFixedSize(10, 10);

//        mStatusLabel->setStyleSheet(utils.borderStyleSheet(colors.STOPPED, colors.WHITE, 0, 5));

//        mStatusLabel->setText("");

//        break;

//    default:

//        break;

//    }

//}


void QTAVPlayer::showMessage() {

    release();

}


void QTAVPlayer::playVOD(QString url, bool isClearOld) {

    if (mAVPlayer) {

        if (url == "0") {

            showMessage();

        } else {

            showVideo();

            mVodUrl = url;
            mVideoCellLayout->setCurrentWidget(mVideoRendererWidget);


            if (isClearOld) {

                mAVPlayer->setMediaEndAction(MediaEndAction_Default);

            } else {

                mAVPlayer->setMediaEndAction(MediaEndAction_KeepDisplay);

            }


            mAVPlayer->stop();

            mAVPlayer->setFile(mVodUrl);

            mAVPlayer->setMediaEndAction(MediaEndAction_KeepDisplay);


            mAVPlayer->play();


        }

    }

}



void QTAVPlayer::stop() {

    if (mAVPlayer) {

        //mAVPlayer->masterClock()->reset();

        mAVPlayer->stop();


    }

}


void QTAVPlayer::pause() {

    if (mAVPlayer) {

        mAVPlayer->pause(!mAVPlayer->isPaused());



    }

}


void QTAVPlayer::play() {

    if (mAVPlayer) {

        mAVPlayer->play();



    }

}


void QTAVPlayer::replay() {

    if (mAVPlayer) {

        //mAVPlayer->masterClock()->reset();

        mAVPlayer->stop();

        //mAVPlayer->masterClock()->start();

        mAVPlayer->play();



    }

}


void QTAVPlayer::release() {

    if (mAVPlayer) {

        mAVPlayer->setMediaEndAction(MediaEndAction_Default);

        mAVPlayer->stop();

        mAVPlayer->setMediaEndAction(MediaEndAction_KeepDisplay);

    }

}




void QTAVPlayer::resizeEvent(QResizeEvent *event) {

    //    qDebug() << event->size();

    //    setSize(event->size());

}

