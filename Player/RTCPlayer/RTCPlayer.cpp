#include "RTCPlayer.h"

// NTBON 3:59 14/9

RTCDownloader *RTCPlayer::getRtpDownloader() const
{
    return rtpDownloader;
}

bool RTCPlayer::getShouldPlayLive() const
{
    return shouldPlayLive;
}

QNetworkAccessManager *RTCPlayer::getNaManager() const
{
    return naManager;
}

NbtpSocket *RTCPlayer::getNbtpSocket() const
{
    return nbtpSocket;
}

bool RTCPlayer::isPlayerPlaying() const
{
    return playerIsPlaying;
}

RTCPlayer::RTCPlayer(QWidget *zone) {
    this->zone = zone;
    this->zone->installEventFilter(this);

    mainLayout = new QVBoxLayout(this->zone);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    this->zone->setLayout(mainLayout);

    topbarSpaceHolder = new QWidget(this->zone);
    topbarSpaceHolder->setStyleSheet("background-color: #111;");
    mainLayout->addWidget(topbarSpaceHolder);

    videoZone = new QWidget(this->zone);
    videoZone->setStyleSheet("background-color: black;");

    mainLayout->addWidget(videoZone);

    topbarSpaceHolder->setFixedHeight(30);

    rtpQueue.setName("RTP Queue" + playerName);
    imageQueue.setName("Image Queue "  + playerName);

    naManager = new QNetworkAccessManager(this);
    rtpDownloader = new RTCDownloader(this, naManager);

    // RTP package builder
    pRTCSplitter = new RTCSplitter(this);

    // LIVE open a socket to receive link continuously and then download it
    if (shouldPlayLive) {
        pRTCSocket = new RTCSocket(this);
        connect(pRTCSocket, &RTCSocket::socketConnected, this, &RTCPlayer::onSocketConnected);
        connect(pRTCSocket, &RTCSocket::socketDisconnected, this, &RTCPlayer::onSocketDisconnected);
        connect(pRTCSplitter, &RTCSplitter::shouldReopenSouce, getPRTCSocket(), &RTCSocket::openSocket);
    } else {
        // PLAYBACK open a LinkManager manage m3u8's link, then download it
        nbtpSocket = new NbtpSocket(this);

        linkManager = new LinkManager(this);
    }

    pRTCRenderer = new RTCRender(this);


    pRTCRendererWidget = new RTCRenderWidget(videoZone);
    pRTCRenderer->setRendererWidget(pRTCRendererWidget);

    //    connect(pRTCRenderer, &RTCRender::startPlaying, this, &RTCPlayer::onPlayerPlaying);
    connect(pRTCRenderer, &RTCRender::showLoadingSign, this, &RTCPlayer::onShowLoadingSign);
    connect(pRTCRenderer, &RTCRender::showPlayerLayer, this, &RTCPlayer::onShowPlayerLayer);
    connect(pRTCRenderer, &RTCRender::showNoCameraLayer, this, &RTCPlayer::onShowNoCameraLayer);
}

RTCPlayer::~RTCPlayer() {
    pRTCRenderer->stopWorking();
    pRTCSplitter->stopWorking();

    delete mainLayout;
    delete pRTCSocket;
    delete pRTCSplitter;
    delete pRTCRenderer;
    delete pRTCRendererWidget;
}

void RTCPlayer::startWorking() {
    //    QString playerName = "Player " + getPlayerName();
    //    qDebug() << playerName << "::RTCPlayer::rtcPlayerStart::START ALL" ;
    playerIsPlaying = true;

    pRTCSplitter->startWorking();
    pRTCRenderer->startWorking();

    if (!shouldPlayLive) {
        // PLAYBACK open a LinkManager manage m3u8's link, then download it
        linkManager->startWorking();
    }
}

void RTCPlayer::stopWorking() {
    //    qDebug() << "RTCPlayer::rtcPlayerStop   BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB  ============ STOP ALL WORKER";
    playerIsPlaying = false;

    pRTCSplitter->stopWorking();
    pRTCRenderer->stopWorking();

    if (!shouldPlayLive) {
        // PLAYBACK open a LinkManager manage m3u8's link, then download it
        linkManager->stopWorking();
    }
}

void RTCPlayer::onSocketConnected() {
}

void RTCPlayer::onSocketDisconnected() {
    qDebug() << "==============================================================================";
    qDebug() << "RTCPlayer::onSocketDisconnected=========================== SOCKET DISCONNECTED";
    qDebug() << "==============================================================================";
}

void RTCPlayer::onShowLoadingSign() {
    Q_EMIT showLoadingSign();
}

void RTCPlayer::onShowPlayerLayer() {
    Q_EMIT showPlayerLayer();
}

void RTCPlayer::onShowNoCameraLayer() {
    Q_EMIT showNoCameraLayer();
}

void RTCPlayer::wheelEventZoomVideo(QVariant *dataStruct) {
    pRTCRendererWidget->wheelEventZoomVideo(dataStruct);
}

void RTCPlayer::eventMoveVideo(QVariant *dataStruct) {
    pRTCRendererWidget->eventMoveVideo(dataStruct);
}

RTCSplitter *RTCPlayer::getPRTCSplitter() const
{
    return pRTCSplitter;
}

QString RTCPlayer::getSourceUrl() const
{
    return sourceUrl;
}

void RTCPlayer::setSourceUrl(const QString &value)
{
    sourceUrl = value;
}

void RTCPlayer::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.PLAYER_TAKE_SCREENSHOT: {
        takeScreenshoot();
    } break;
    case Message.APP_NETWORK_IS_REACHABLE:
    case Message.APP_NETWORK_IS_UNREACHABLE: {
        this->getPRTCSocket()->newAction(message, attachment);
    } break;

    case Message.PLAYER_SOURCE_CLEAR: {
        this->setSourceUrl("");
        if (shouldPlayLive)
            //this->pRTCSocket->closeSource();
            this->pRTCSocket->newAction(message, attachment);
        stopWorking();
    } break;

    case Message.PLAYER_PLAY_LIVE_SD: {
        this->camItem->setIsMain(false);
        playSource();
        Q_EMIT showLoadingSign();

    } break;

    case Message.PLAYER_PLAY_LIVE_HD: {
        this->camItem->setIsMain(true);
        playSource();
        Q_EMIT showLoadingSign();
    } break;

    case Message.PLAYER_NEW_LIVE_SOURCE_SET: {
        this->camItem = attachment->value<CamItem *>();
        playSource();
    } break;

    case Message.HIDE: {
        this->zone->hide();
    } break;

    case Message.SHOW: {
        this->zone->show();
    } break;

    case Message.WHEEL_EVENT_ZOOM_VIDEO: {
        wheelEventZoomVideo(attachment);
    } break;

    case Message.EVENT_MOVE_ZOOM_VIDEO: {
        eventMoveVideo(attachment);
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in RTCPlayer "
                 << "non-catched :" + Message.toString(message);
    }
}

void RTCPlayer::playSource() {
    startWorking();
    networkType = this->camItem->getNetworkType();
    networkType.name = this->camItem->getIsMain() ? "HD" : "SD";
    if (this->camItem != Q_NULLPTR) {
        QString newSource;
        CamStream *camStream = camItem->getCamStream(this->networkType);
        if (camStream){
            newSource = camStream->getSource();
            if (newSource.isEmpty()) return;
            if (!this->sourceUrl.isEmpty() && !newSource.isEmpty()) {
                if (this->sourceUrl == newSource) {
                    qDebug() << playerName << ":::P_Cam9RTCPlayer::setNewSource::SAME SOURCE";
                    return; // do nothing with the same same source
                }
            }
        }
        qDebug() << "PLAY SOURCE" << newSource ;
        this->pRTCSocket->openSource(newSource);
    } else {
        Q_EMIT showNoCameraLayer();
    }
}

void RTCPlayer::takeScreenshoot() {
    if (camItem != Q_NULLPTR) {
        QImage image;
        image = this->zone->grab().toImage();

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

bool RTCPlayer::eventFilter(QObject *watched, QEvent *event) {
    QWidget *sender = qobject_cast<QWidget *>(watched);
    if (sender != Q_NULLPTR && sender == this->zone) {
        switch (event->type()) {
        case QEvent::Resize: {
            QResizeEvent *resizeEvent = (QResizeEvent *)(event);
            QSize newSize = resizeEvent->size();
            this->videoZone->resize(newSize);
            topbarSpaceHolder->setFixedHeight(30);
            videoZone->move(0,30);
            this->videoZone->resize(newSize.width(), newSize.height() - 30);
        } break;

        default:
            break;
        }
    }
    return true;
}
