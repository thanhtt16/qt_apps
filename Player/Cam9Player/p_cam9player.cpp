#include "p_cam9player.h"
#include "Common/LayoutSet.h"

P_Cam9Player::P_Cam9Player(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    this->zone = zone;
    this->zone->installEventFilter(this);
    initUI();
}

QString P_Cam9Player::getModeOneVideoVOD() const
{
    return modeOneVideoVOD;
}

void P_Cam9Player::setModeOneVideoVOD(const QString &value)
{
    modeOneVideoVOD = value;
}

void P_Cam9Player::initUI() {
    //    mainLayout = new QStackedLayout();
    //    mainLayout->setMargin(0);
    //    mainLayout->setAlignment(Qt::AlignCenter);
    //    this->zone->setLayout(mainLayout);

    //    //     init mpv player
    //    mpvPlayer = new MpvWidget(this->zone);

    //    //    topbarSpaceHolder = new QWidget(this->zone);
    //    //    topbarSpaceHolder->setFixedHeight(30);
    //    //    topbarSpaceHolder->setStyleSheet("background-color: #111;");
    //    //    mainLayout->addWidget(topbarSpaceHolder);

    //    mainLayout->addWidget(mpvPlayer);

    //    connect(mpvPlayer, &MpvWidget::playerPlaying, this,
    //            &P_Cam9Player::onPlayerPlaying);
    //    connect(mpvPlayer, &MpvWidget::playerPaused, this,
    //            &P_Cam9Player::onPlayerPaused);
    //    connect(mpvPlayer, &MpvWidget::playerReplay, this,
    //            &P_Cam9Player::onPlayerReplay);

    //    // init black zone
    //    this->blackZone = new QWidget(this->zone);
    //    this->blackZone->setStyleSheet("background-color: #80008000");
    //    //  this->blackZone->setStyleSheet("background-color: black");
    //    this->blackZone->move(0, 0);
    //    this->blackZone->resize(this->zone->size());
    //    this->blackZone->hide();

    //    this->overlayZone = new QWidget(this->zone);
    //    overlayZone->show();
    //    overlayZone->raise();


    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignTop);
    this->zone->setLayout(mainLayout);
    this->zone->setStyleSheet("background:green");
    int heightTop = 30;
    //     init mpv player

    topbarSpaceHolder = new QWidget(this->zone);
    QVBoxLayout *topLayout = new QVBoxLayout();
    topbarSpaceHolder->setLayout(topLayout);
    topbarSpaceHolder->setFixedHeight(heightTop);
    topLayout->setMargin(0);
    topLayout->setSpacing(0);
    topbarSpaceHolder->setStyleSheet("background-color: #111;");

    topbarSpaceHolder->move(0,30);
    topbarSpaceHolder->resize(QSize(this->zone->width() , heightTop));

    mpvWidget = new QWidget(this->zone);
    mpvWidget->setStyleSheet("background :red");
    mpvWidget->move(0,30);
    mpvWidget->resize(QSize(this->zone->width() , this->zone->height() - heightTop));
    //add layout main
    mainLayout->addWidget(topbarSpaceHolder);
    mainLayout->addWidget(mpvWidget);

    //layout bot
    mpvLayoutStack = new QStackedLayout();
    mpvLayoutStack->setContentsMargins(0, 0, 0, 0);
    mpvLayoutStack->setSpacing(0);
    mpvWidget->setLayout(mpvLayoutStack);

    mpvPlayer = new MpvWidget(mpvWidget);
    mpvPlayer->setVideoAspect(mpvWidget->size());

    //    connect(mpvPlayer, &MpvWidget::playerPaused, this,
    //            &P_Cam9Player::onPlayerPaused);
    connect(mpvPlayer, &MpvWidget::playerPlaying, this,
            &P_Cam9Player::onShowPlayerLayer); //neu dang chay hien player len
    connect(mpvPlayer, &MpvWidget::playerLoading, this,
            &P_Cam9Player::onShowLoadingSign); //neu dang khong chay
    connect(mpvPlayer, &MpvWidget::playerReplay, this,
            &P_Cam9Player::onPlayerReplay);



    //     1. WALL LAYER
    this->wallZone = new QWidget(this->zone);
    QVBoxLayout *wallZoneLayout = new QVBoxLayout();
    wallZoneLayout->setAlignment(Qt::AlignCenter);
    this->wallZone->setLayout(wallZoneLayout);
    this->wallZone->setStyleSheet("background-color: #0E6A6B");
    this->wallZone->move(0,0);
    this->wallZone->resize(this->zone->size());


    QLabel *noCameraMessage = new QLabel(this->wallZone);
    noCameraMessage->setAlignment(Qt::AlignCenter);
    noCameraMessage->setStyleSheet("background-color: #00000000");
    noCameraMessage->setMinimumSize(50, 50);
    noCameraMessage->setText("NO CAMERA");
    noCameraMessage->setFont(Resources::instance().getMediumBoldButtonFont());
    wallZoneLayout->addWidget(noCameraMessage);

    // init black zone
    this->blackZone = new QWidget(this->mpvWidget);
    this->blackZone->setStyleSheet("background-color: #0E6A6B");
    this->blackZone->resize(mpvWidget->size());
    //    this->blackZone->setStyleSheet("background-color: #80008000");
    //  this->blackZone->setStyleSheet("background-color: black");
    this->overlayZone = new QWidget(this->zone);
    this->overlayZone->move(0,0);
    this->overlayZone->resize(this->zone->size());

    mpvLayoutStack->addWidget(blackZone);
    mpvLayoutStack->addWidget(mpvPlayer);
    this->mpvLayoutStack->setCurrentWidget(this->mpvPlayer); //lan dau tien chua chay
    this->overlayZone->show();
    this->overlayZone->raise();
}

QWidget *P_Cam9Player::getZone(int zoneId) {
    switch (zoneId) {
    case 0:
        return this->zone;

    case 1:
        return this->overlayZone;

    case 2:
        return this->blackZone;

    default:
        return Q_NULLPTR;
    }
}


void P_Cam9Player::cam9Play() {
    QString playerName = "Player " + this->control()->getDisplayName();
    this->mpvPlayer->play();
}

void P_Cam9Player::cam9Stop() {
    QString playerName = "Player " + this->control()->getDisplayName();
    this->onShowWallLayer();
    this->mpvPlayer->stop();
    this->url = "";
}

void P_Cam9Player::cam9Pause() {
    if (this->isLive) {
        this->mpvPlayer->pause();
    }
}

void P_Cam9Player::playHD() {
    cam9Play(this->videoVod);
}

void P_Cam9Player::playSD() {
    cam9Play(this->videoVod);
}

void P_Cam9Player::cam9Play(VideoVOD *videoVod) {
    this->isLive = false;
    if (videoVod != Q_NULLPTR) {
        this->videoVod = videoVod;
        this->speedAllVideo = this->videoVod->getSpeedVideo();
        this->camItem = this->videoVod->getCamItem();
        this->camItem->setIsLive(this->isLive);
        this->order = this->camItem->getOrder();
        this->isMain = this->camItem->getIsMain();
        if (this->camItem->isTv()) {
            //namtv
            this->versionCurrent =
                    AppProfile::getAppProfile()->getAppConfig()->getApiVodVersion();
            QString urlSub, urlMain;
            if (versionCurrent == "2.0") {
                urlMain = this->videoVod->getVideoVODProfile()->getSrc();
                urlSub = convertUrlMainToSub(urlMain);
            } else if (versionCurrent == "3.0") {
                urlMain = this->videoVod->getSrcMain();
                urlSub = this->videoVod->getSrcSub();
                qDebug() << Q_FUNC_INFO << urlMain << urlSub;
            }

            if (this->videoVod->getModeCurrent() == "SD") {
                this->isMain = false;
                this->camItem->setIsMain(this->isMain);
                playVOD(urlSub, this->speedAllVideo);
            } else {
                this->isMain = true;
                this->camItem->setIsMain(this->isMain);
                playVOD(urlMain, this->speedAllVideo);
            }
        } else {
            //namcdn
            if (this->videoVod->getModeCurrent() == "HD") {
                this->isMain = true;
            } else if (this->videoVod->getModeCurrent() == "SD") {
                this->isMain = false;
            }

            this->camItem->setIsMain(this->isMain);
            //get link
            //            this->camItem->getAvailableCamStream(
            //                        this->isMain,
            //                        [this](CamStream *camStream) {
            //                if (camStream != Q_NULLPTR) {
            //                    if (this->camItem != Q_NULLPTR) {
            //                        playVOD(liveToVod(camStream->getSource(),
            //                                          this->camItem->getTimeRange()),
            //                                this->speedAllVideo *
            //                                this->videoVod->getSpeedScale());
            //                    }
            //                }
            //            },
            //            [this] {});

            networkTypeWorking = camItem->getNetworkType();
            networkTypeWorking.name = this->isMain ? "HD" : "SD";
            CamStream *camStream =  this->camItem->getCamStream(networkTypeWorking);
            if(camStream){
                playVOD(liveToVod(camStream->getSource(),
                                  this->camItem->getTimeRange()),
                        this->speedAllVideo *
                        this->videoVod->getSpeedScale());
            }

        }

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(this->camItem);
        control()->newAction(Message.PLAYER_UPDATE_INFO, dataStruct);
    } else {
        cam9Stop();
    }
}

void P_Cam9Player::playVOD(QString url, double speed) {
    QString playerName = "Player " + this->control()->getDisplayName();
    if (!url.isEmpty() && mpvPlayer) {
        if(this->url != url){
            onShowLoadingSign();
            this->url = url;
            this->isLive = false;
            this->mpvPlayer->setSpeed(speed);
            this->mpvPlayer->setMainStream(this->isMain);
            this->mpvPlayer->open(url, this->isLive, this->isMain);
        }else{
            return;
        }
    } else {
    }
}


void P_Cam9Player::playOneVideoVod(VideoVOD *videoVod) {
    this->isLive = false;
    if (videoVod != Q_NULLPTR) {
        this->videoVod = videoVod;
        this->camItem = this->videoVod->getCamItem();
        this->camItem->setIsLive(this->isLive);
        this->camItem->setIsMain(this->isMain);
        this->order = this->camItem->getOrder();
        // this->mpvPlayer->setOrder(this->order);

        if (this->camItem->isTv()) {
            this->versionCurrent =
                    AppProfile::getAppProfile()->getAppConfig()->getApiVodVersion();
            QString urlSub, urlMain;
            if (versionCurrent == "2.0") {
                urlMain = this->videoVod->getVideoVODProfile()->getSrc();
                urlSub = convertUrlMainToSub(urlMain);
            } else if (versionCurrent == "3.0") {
                urlMain = this->videoVod->getSrcMain();
                urlSub = this->videoVod->getSrcSub();
            }

            if (modeOneVideoVOD == "SD") {
                this->isMain = false;
                this->camItem->setIsMain(this->isMain);
                playVOD(urlSub, this->speedAllVideo);
            } else {
                this->isMain = true;
                this->camItem->setIsMain(this->isMain);
                playVOD(urlMain, this->speedAllVideo);
            }
        } else {
            if (modeOneVideoVOD == "HD") {
                this->isMain = true;
            } else if (modeOneVideoVOD == "SD") {
                this->isMain = false;
            }
            this->camItem->setIsMain(this->isMain);
            //            this->camItem->getAvailableCamStream(
            //                        this->isMain,
            //                        [this](CamStream *camStream) {
            //                if (camStream != Q_NULLPTR) {
            //                    playVOD(liveToVod(camStream->getSource(),
            //                                      this->camItem->getTimeRange()),
            //                            this->speedAllVideo *
            //                            this->videoVod->getSpeedScale());
            //                }
            //            },
            //            [this] {});

            networkTypeWorking = camItem->getNetworkType();
            networkTypeWorking.name = this->isMain ? "HD" : "SD";
            CamStream *camStream =  this->camItem->getCamStream(networkTypeWorking);
            if(camStream){
                playVOD(liveToVod(camStream->getSource(),
                                  this->camItem->getTimeRange()),
                        this->speedAllVideo *
                        this->videoVod->getSpeedScale());
            }
        }
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(this->camItem);
        control()->newAction(Message.PLAYER_UPDATE_INFO, dataStruct);
    } else {
        cam9Stop();
    }
}



QString P_Cam9Player::liveToVod(QString live, TimeRange timeRange) {
    QString vod = "http://";
    QStringList lives = live.split("/");
    vod.append(lives.at(2));
    vod.append("/rec/hls/");
    vod.append(lives.last());
    vod.append("_");
    vod.append(QString::number(timeRange.starttime * 1000));
    vod.append("_");
    vod.append(QString::number(timeRange.length * 1000));
    vod.append("_h264.m3u8");
    return vod;
}




void P_Cam9Player::playVodHd() {
    this->isMain = true;
    modeOneVideoVOD = "HD";
}

void P_Cam9Player::playVodSd() {
    this->isMain = false;
    modeOneVideoVOD = "SD";
}

void P_Cam9Player::playBack() { playVOD("", 1.0); }

void P_Cam9Player::onPlayerReplay() {
    //10s after stop
    QString playerName = "Player " + this->control()->getDisplayName();
    if(this->camItem && this->camItem->isRtc()){
        CamStream *camStream =  this->camItem->getCamStream(networkTypeWorking);
        if(camStream){
            qDebug() << "Continue ->" << playerName;
            playVOD(liveToVod(camStream->getSource(),timerangeCurrent),
                    this->speedAllVideo *
                    this->videoVod->getSpeedScale());
        }
    }
}

void P_Cam9Player::setSize(QSize size) {
    videoSize = size;
    mpvPlayer->move(0,0);
    mpvPlayer->resize(QSize(mpvWidget->width() , mpvWidget->height()));
    mpvPlayer->setVideoAspect(QSize(mpvWidget->width() , mpvWidget->height()));
    this->blackZone->move(0, 0);
    blackZone->resize(QSize(mpvWidget->width() , mpvWidget->height()));
}

void P_Cam9Player::adjustLayout(QSize size) {
    this->wallZone->move(0,0);
    this->wallZone->resize(size);
    this->topbarSpaceHolder->move(0,0);
    topbarSpaceHolder->resize(QSize(size.width(), 30));
    this->mpvWidget->move(0,30);
    this->mpvWidget->resize(QSize(size.width(), size.height() - 30));
    this->mpvWidget->updateGeometry();

    this->overlayZone->move(0, 0);
    this->overlayZone->resize(size);
    setSize(size);

}


void P_Cam9Player::updateSizeWhenEnterFullScreenMode(){
    QSize sizeUpdate = this->zone->size();
    this->zone->resize(QSize(sizeUpdate.width() + 420 ,sizeUpdate.height() +40));
    this->mpvWidget->move(0, 0);
    this->mpvWidget->resize(QSize(sizeUpdate.width() + 420 ,sizeUpdate.height() +40));
    this->mpvWidget->updateGeometry();
}


void P_Cam9Player::updateSizeWhenExitFullScreenMode(){
    QSize sizeUpdate = this->zone->size();
    this->zone->resize(QSize(sizeUpdate.width() - 420 ,sizeUpdate.height() - 40));
    this->mpvWidget->move(0, 0);
    this->mpvWidget->resize(QSize(sizeUpdate.width() - 420 ,sizeUpdate.height() - 40));
    this->mpvWidget->updateGeometry();
}

bool P_Cam9Player::eventFilter(QObject *watched, QEvent *event) {
    QWidget *sender = qobject_cast<QWidget *>(watched);
    if (sender != Q_NULLPTR && sender == this->zone) {
        switch (event->type()) {
        case QEvent::Resize: {
            QResizeEvent *resizeEvent = (QResizeEvent *)(event);
            QSize newSize = resizeEvent->size();
            this->adjustLayout(newSize);
        } break;

        default:
            break;
        }
    }
    return true;
}

void P_Cam9Player::seekToNewPostion(int newPosition) {
    mpvPlayer->seek(newPosition);
    mpvPlayer->play();
}

QString P_Cam9Player::convertUrlMainToSub(QString urlMain) {
    QString urlSub;
    if (urlMain != "") {
        QStringList urlSubList = urlMain.split("/");
        if (urlSubList.length() > 5) {
            QString httpString = urlSubList[0];
            QString baseUrl = urlSubList[2];
            QString catchUpString = urlSubList[3];
            QString addStringSub = urlSubList[4];
            urlSub = httpString.append("//")
                    .append(baseUrl)
                    .append("/")
                    .append(catchUpString)
                    .append("/")
                    .append(addStringSub)
                    .append("S/")
                    .append(urlSubList[5]);
        } else {
            urlSub = urlMain;
        }
    } else {
        urlSub = urlMain;
    }
    return urlSub;
}

void P_Cam9Player::takeScreenshoot() {
    if (camItem != Q_NULLPTR) {

        QImage image = this->mpvPlayer->grab().toImage();

        QString currentDateTime =
                QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        currentDateTime = currentDateTime.replace("/", "-");
        currentDateTime = currentDateTime.replace(":", "-");
        QString defaultFileName =
                QString("%1 %2").arg(this->camItem->getPostion()).arg(currentDateTime);



        QSettings settings;
        //        settings.beginGroup(QString::number(control()->appContext->getWorkingUser()->getUserId()));
        QString pathSaveMedia = settings.value("path_save_media").toString();
        QString fileName;
        if (pathSaveMedia == "") {
            fileName = QFileDialog::getSaveFileName(
                        Q_NULLPTR, tr("Save Video Wall Screenshot"), defaultFileName,
                        tr("PNG (*.png);;All Files (*)"));
        } else {
            fileName = pathSaveMedia + "/" + defaultFileName;
        }
        if (fileName.isEmpty()) {
            return;
        } else {
            if (!fileName.contains(".png")) {
                fileName.append(".png");
            }
            image.save(fileName);
        }
    }
}

void P_Cam9Player::setSpeed(double speed) {
    this->speedAllVideo = speed;
    double scale = 1.0;
    if (this->videoVod != Q_NULLPTR) {
        scale = this->videoVod->getSpeedScale();
    }
    mpvPlayer->setSpeed(this->speedAllVideo * scale);
}

void P_Cam9Player::wheelEventZoomVideo(QVariant *dataStruct) {
    DataZoomVideo data = dataStruct->value<DataZoomVideo>();
    qreal deg = data.deg;
    if (deg > 0) {
        mpvPlayer->zoomIn();
    } else {
        mpvPlayer->zoomOut();
    }
}

void P_Cam9Player::eventMoveVideo(QVariant *dataStruct) {
}


void P_Cam9Player::onShowLoadingSign() {
    QString playerName = "Player " + this->control()->getDisplayName();
    control()->newAction(Message.PLAYER_LOADING, Q_NULLPTR);
    wallZone->hide();
    mpvPlayer->hide();
    blackZone->show();
    blackZone->raise();
}


void P_Cam9Player::onShowPlayerLayer() {
    QString playerName = "Player " + this->control()->getDisplayName();
    control()->newAction(Message.PLAYER_PLAYING, Q_NULLPTR);
    wallZone->hide();
    blackZone->hide();
    mpvPlayer->show();
    mpvPlayer->raise();
    overlayZone->show();
    overlayZone->raise();
}

void P_Cam9Player::onShowWallLayer() {
    QString playerName = "Player " + this->control()->getDisplayName();
    //   control()->newUserAction(Message.PLAYER_STOPED, Q_NULLPTR);
    mpvPlayer->hide();
    overlayZone->hide();
    blackZone->hide();
    wallZone->show();
    wallZone->raise();
}

void P_Cam9Player::updateTimestampCurrent(TimeRange timestampCurrent){
    timerangeCurrent = timestampCurrent;
}



//void P_Cam9Player::onPlayerLoading() {
//    QString playerName = "Player " + this->control()->getDisplayName();

//    //    control()->newAction(Message.PLAYER_LOADING, Q_NULLPTR);

//    blackZone->show();
//    blackZone->raise();
//    overlayZone->show();
//    overlayZone->raise();
//}

//void P_Cam9Player::onPlayerPlaying() {
//    QString playerName = "Player " + this->control()->getDisplayName();

//    //    control()->newAction(Message.PLAYER_PLAYING, Q_NULLPTR);
//    wallZone->hide();
//    blackZone->hide();
//    overlayZone->show();
//    overlayZone->raise();
//}

//void P_Cam9Player::onPlayerPaused() {
//    QString playerName = "Player " + this->control()->getDisplayName();
//    //    control()->newAction(Message.PLAYER_PAUSED, Q_NULLPTR);

//}

//void P_Cam9Player::onPlayerStopped() {
//    QString playerName = "Player " + this->control()->getDisplayName();

//    //    control()->newAction(Message.PLAYER_STOPED, Q_NULLPTR);
//    blackZone->hide();
//    overlayZone->hide();
//    wallZone->show();
//    wallZone->raise();
//}
