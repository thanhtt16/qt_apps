#include "PlaybackPlayer.h"

// NTBON 3:59 14/9

M3U8ToQueue* PlaybackPlayer::getM3u8ToQueue() const
{
    return m3u8ToQueue;
}

PlaybackDownloader *PlaybackPlayer::getDownloader() const
{
    return downloader;
}

PlaybackPlayer::PlaybackPlayer(QWidget *parent) : QWidget(parent) {
    // get link
    pRTCSocket = new PlaybackSocket(this);

    connect(pRTCSocket, &PlaybackSocket::socketConnected, this, &PlaybackPlayer::onSocketConnected);
    connect(pRTCSocket, &PlaybackSocket::socketDisconnected, this, &PlaybackPlayer::onSocketDisconnected);

    // m3u8
    m3u8ToQueue = new M3U8ToQueue(this);
    //m3u8ToQueue->setNewSource("https://2c1.vcam.viettel.vn/rec/hls/d02212d8c107xyz13592_1506700800000_3600000_nbtp.m3u8");

    downloader = new PlaybackDownloader(this);

    pM3U8Downloader = new M3U8Linker(this);
    connect(pM3U8Downloader, &M3U8Linker::playBackDownload, this, &PlaybackPlayer::onReceiveDowloadRTCData);
    pM3U8Downloader->startWorking();

    // RTP package builder
    pRTCSplitter = new PlaybackSplitter(this);
    pRTCSplitter->startWorking();
    connect(pRTCSplitter, &PlaybackSplitter::shouldReopenSouce, getPRTCSocket(), &PlaybackSocket::openSocket);

    pRTCRenderer = new PlaybackRender(this);
    pRTCRenderer->startWorking();

    pRTCRendererWidget = new PlaybackRenderWidget(this);
    pRTCRenderer->setRendererWidget(pRTCRendererWidget);

//    connect(pRTCRenderer, &RTCRender::startPlaying, this, &RTCPlayer::onPlayerPlaying);
    connect(pRTCRenderer, &PlaybackRender::showLoadingSign, this, &PlaybackPlayer::onShowLoadingSign);
    connect(pRTCRenderer, &PlaybackRender::showPlayerLayer, this, &PlaybackPlayer::onShowPlayerLayer);
    //connect(pRTCRenderer, &RTCRender::hidePlayerNoCamera, this, &RTCPlayer::showNoCameraLayer);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    QWidget *topbarSpaceHolder = new QWidget(this);
    topbarSpaceHolder->setFixedHeight(30);
    topbarSpaceHolder->setStyleSheet("background-color: #111;");

    mainLayout->addWidget(topbarSpaceHolder);
    mainLayout->addWidget(pRTCRendererWidget);
    pRTCRendererWidget->raise();
    pRTCRendererWidget->show();

    rtpQueue.setName("RTP Queue" + playerName);
    imageQueue.setName("Image Queue "  + playerName);

    type.protocol = "WS";
    type.network = "CDN";
    type.name = "SD";
}

PlaybackPlayer::~PlaybackPlayer() {
    pRTCRenderer->stopWorking();
    pRTCSplitter->stopWorking();

    delete mainLayout;
    delete pRTCSocket;
    delete pRTCSplitter;
    delete pRTCRenderer;
    delete pRTCRendererWidget;
}

void PlaybackPlayer::onReceiveDowloadRTCData(quint64 linkIndex, QString url) {
    downloader->onReceiveDowloadRTCData(linkIndex, url);
}

void PlaybackPlayer::rtcPlayerStart() {
    QString playerName = "Player " + getPlayerName();
    qDebug() << playerName << "::PlaybackPlayer::rtcPlayerStart::START ALL" ;
    pRTCSocket->openSource(this->sourceUrl);
}

void PlaybackPlayer::rtcPlayerPlay() {
    QString playerName = "Player " + getPlayerName();
    qDebug() << playerName << "::PlaybackPlayer::rtcPlayerPlay <> Start" ;

    pRTCSocket->openSource(this->sourceUrl);

    //pRTCSocket->connectSocket();
    //onPlayerLoading();
}

void PlaybackPlayer::rtcPlayerStop() {
    qDebug() << "PlaybackPlayer::rtcPlayerStop   BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB  ============ STOP ALL WORKER";

    setSourceUrl("");

    //onPlayerStopped();
}


void PlaybackPlayer::resetQueues() {
}

void PlaybackPlayer::onSocketConnected() {
}

void PlaybackPlayer::onSocketDisconnected() {
    qDebug() << "==============================================================================";
    qDebug() << "RTCPlayer::onSocketDisconnected=========================== SOCKET DISCONNECTED";
    qDebug() << "==============================================================================";
}

void PlaybackPlayer::onShowLoadingSign() {
    Q_EMIT showLoadingSign();
}

void PlaybackPlayer::onShowPlayerLayer() {
    Q_EMIT showPlayerLayer();
}

void PlaybackPlayer::onShowWallLayer() {
    Q_EMIT showWallLayer();
}

void PlaybackPlayer::zoomVideo(QVariant *dataStruct) {
    pRTCRendererWidget->wheelEventZoomVideo(dataStruct);
}

void PlaybackPlayer::eventMoveVideo(QVariant *dataStruct) {
    pRTCRendererWidget->eventMoveVideo(dataStruct);
}

PlaybackSplitter *PlaybackPlayer::getPRTCSplitter() const
{
    return pRTCSplitter;
}

QString PlaybackPlayer::getSourceUrl() const
{
    return sourceUrl;
}

void PlaybackPlayer::setSourceUrl(const QString &value)
{
    sourceUrl = value;
}

void PlaybackPlayer::newAction(int message, QVariant *attachment) {
    switch (message) {
    case Message.APP_NETWORK_IS_REACHABLE:
    case Message.APP_NETWORK_IS_UNREACHABLE: {
        this->getPRTCSocket()->newAction(message, attachment);
    } break;

    case Message.PLAYER_SOURCE_CLEAR: {
        this->setSourceUrl("");
        this->pRTCSocket->closeSource();
    } break;

    case Message.PLAYER_PLAY_LIVE_SD: {
        this->camItem->setIsMain(false);
        playSource();
    } break;

    case Message.PLAYER_PLAY_LIVE_HD: {
        this->camItem->setIsMain(true);
        playSource();
    } break;

    case Message.PLAYER_NEW_LIVE_SOURCE_SET: {
        this->camItem = attachment->value<CamItem *>();
        playSource();
    } break;

    default:
        qDebug() << "ERROR : General Internal pac action in RTCPlayer "
                 << "non-catched :" + Message.toString(message);
    }
}

void PlaybackPlayer::playSource() {
    type.name = this->camItem->getIsMain() ? "HD" : "SD";
    if (this->camItem != Q_NULLPTR) {
        CamStream *camStream = camItem->getCamStream(this->type);
        if (!camStream) return;
        QString newSource = camStream->getSource();
        if (newSource.isEmpty()) return;

        if (!this->sourceUrl.isEmpty() && !newSource.isEmpty()) {
            if (this->sourceUrl == newSource) {
                qDebug() << playerName << ":::P_Cam9RTCPlayer::setNewSource::SAME SOURCE";
                return; // do nothing with the same same source
            }
        }
        //this->pRTCSocket->openSource(newSource);
        makePlaybackSource(newSource);
    } else {
        Q_EMIT showWallLayer();
    }
}

void PlaybackPlayer::makePlaybackSource(QString source) {
    QString host ="";
    QString mac ="";
    QString cam ="";

    //buildLinkFormat for download link
    QString tmpUrl = source;
    tmpUrl.replace("//", "/");
    QStringList splitedUrl = tmpUrl.split("/");
    if (splitedUrl.length() == 5) {
        host = splitedUrl.at(1);
        mac = splitedUrl.at(3);
        cam = splitedUrl.at(4);
    }
    // playback
    QString playBackSource = QString("http://%1/rec/hls/%2_%3_%4_h264.m3u8").arg(host).arg(cam).arg("1506593410").arg("732000");
    qDebug() << "=======================================================" << playBackSource;
     playBackSource = "https://2c1.vcam.viettel.vn/rec/hls/d02212d91df5xyz20070_1506736800000_3600000_h264.m3u8";
    this->m3u8ToQueue->setNewSource(playBackSource);
}
