#include "p_cam9rtcplayer.h"
#include "Common/LayoutSet.h"

P_Cam9RTCPlayer::P_Cam9RTCPlayer(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    this->zone = zone;
    this->zone->installEventFilter(this);

    //initUI
    mainLayout = new QStackedLayout();
    mainLayout->setMargin(0);
    mainLayout->setAlignment(Qt::AlignCenter);
    this->zone->setLayout(mainLayout);

    // 1. WALL LAYER
    this->wallZone = new QWidget(this->zone);

    QVBoxLayout *wallZoneLayout = new QVBoxLayout();
    wallZoneLayout->setAlignment(Qt::AlignCenter);
    this->wallZone->setLayout(wallZoneLayout);
    this->wallZone->setStyleSheet("background-color: #0E6A6B");
    this->wallZone->move(0, 0);
    this->wallZone->resize(this->zone->size());
    //    wallZone->hide();

    QLabel *noCameraMessage = new QLabel(this->wallZone);
    noCameraMessage->setAlignment(Qt::AlignCenter);
    noCameraMessage->setStyleSheet("background-color: #00000000");
    noCameraMessage->setMinimumSize(50, 50);
    noCameraMessage->setText("NO VIDEO");
    noCameraMessage->setFont(Resources::instance().getMediumBoldButtonFont());
    wallZoneLayout->addWidget(noCameraMessage);

    // 2. PLAYER LAYER
    playerZone = new QWidget(this->zone);

    //    QVBoxLayout *playerZoneLayout = new QVBoxLayout();
    //    playerZoneLayout->setAlignment(Qt::AlignCenter);
    //this->playerZone->setLayout(playerZoneLayout);
    this->playerZone->setStyleSheet("background-color: #000000");
    this->playerZone->move(0, 0);
    this->playerZone->resize(this->zone->size().width(), this->zone->height());
    //    this->playerZone->show();

    // 3. RIDEAX LAYER
    this->blackZone = new QWidget(this->zone);

    QVBoxLayout *blackZoneLayout = new QVBoxLayout();
    blackZoneLayout->setAlignment(Qt::AlignCenter);
    this->blackZone->setLayout(blackZoneLayout);
    this->blackZone->setStyleSheet("background-color: #0E6A6B");
    //    this->blackZone->setStyleSheet("background-color: #black");

    //this->blackZone->setStyleSheet("background-color: #008000");
    //this->blackZone->setStyleSheet("background-color: black");
    this->blackZone->move(0, 0);
    this->blackZone->resize(this->zone->size());
    //    blackZone->hide();

    // 4. OVERLAY LAYER
    this->overlayZone = new QWidget(this->zone);
    this->overlayZone->move(0, 0);
    this->overlayZone->resize(this->zone->size());

    mainLayout->addWidget(wallZone);
    mainLayout->addWidget(blackZone);
    mainLayout->addWidget(playerZone);
    mainLayout->setCurrentWidget(playerZone);
}

QWidget *P_Cam9RTCPlayer::getZone(int zoneId) {
    switch (zoneId) {

    case 0:
        return this->zone;

    case 1:
        return this->overlayZone;

    case 2:
        return this->blackZone;

    case 3: return this->playerZone;

    default:
        return Q_NULLPTR;
    }
}

void P_Cam9RTCPlayer::clearSource() {
    QString playerName = "Player " + this->control()->getDisplayName();
    qDebug() << playerName << ":::clearSource";
    this->url = "";
    playerZone->hide();
    overlayZone->hide();
    blackZone->hide();

    wallZone->show();
    wallZone->raise();
}

void P_Cam9RTCPlayer::showLoadingZone(){

    wallZone->hide();
    playerZone->hide();

    blackZone->show();
    blackZone->raise();

    overlayZone->show();
    overlayZone->raise();
}


//void P_Cam9RTCPlayer::setNewSource(CamItem *camItem) {
//    QString playerName = "Player " + this->control()->getDisplayName();
//    qDebug() << playerName << ":::setNewSource";

//    if (camItem != Q_NULLPTR) {
//        CamItemType type;
//        type.protocol = "WS";
//        type.network = "CDN";
//        type.name = "SD";
//        QString newSource = camItem->getCamStream(type)->getSource();

//        if (!this->url.isEmpty()) {
//            if (this->url == newSource) {
//                qDebug() << playerName << ":::P_Cam9RTCPlayer::setNewSource::SAME SOURCE";
//                return; // do nothing with the same same source
//            }
//            this->rtcPlayer->rtcPlayerStop();
//        }
//        this->camItem = camItem;
//        this->order = this->camItem->getOrder();
//        this->url = newSource;
//        this->rtcPlayer->setSourceUrl(newSource);
//        this->rtcPlayer->rtcPlayerStart();

//        QVariant *dataStruct = new QVariant();
//        dataStruct->setValue(this->camItem);
//        control()->newAction(Message.PLAYER_UPDATE_INFO, dataStruct);
//    } else {
//        qDebug() << playerName << "can not play NULL camItem";
//    }
//}

void P_Cam9RTCPlayer::playLive(QString url) {
    this->url = url;
    qDebug() << "P_Cam9RTCPlayer::playLive   AAAAAAAA  =========== 3333333 " << url;

    QString playerName = "Player " + this->control()->getDisplayName();
    this->isLive = true;
}

void P_Cam9RTCPlayer::onShowLoadingSign() {
    QString playerName = "Player " + this->control()->getDisplayName();
    control()->newUserAction(Message.PLAYER_LOADING, Q_NULLPTR);

    wallZone->hide();
    playerZone->hide();

    blackZone->show();
    blackZone->raise();

    overlayZone->show();
    overlayZone->raise();
}

void P_Cam9RTCPlayer::onShowPlayerLayer() {
    QString playerName = "Player " + this->control()->getDisplayName();
    control()->newAction(Message.PLAYER_PLAYING, Q_NULLPTR);
    wallZone->hide();
    blackZone->hide();

    playerZone->show();
    playerZone->raise();

    overlayZone->show();
    overlayZone->raise();
}

void P_Cam9RTCPlayer::onShowWallLayer() {
    QString playerName = "Player " + this->control()->getDisplayName();
    //   control()->newUserAction(Message.PLAYER_STOPED, Q_NULLPTR);

    playerZone->hide();
    overlayZone->hide();
    blackZone->hide();

    wallZone->show();
    wallZone->raise();
}

void P_Cam9RTCPlayer::setSize(QSize size) {
    videoSize = size;
}

void P_Cam9RTCPlayer::adjustLayout(QSize size) {
    this->wallZone->move(0, 0);
    this->wallZone->resize(size);

    this->blackZone->move(0, 0);
    this->blackZone->resize(size);

    this->playerZone->move(0, 0);
    this->playerZone->resize(size);

    this->overlayZone->move(0, 0);
    this->overlayZone->resize(size);
}

bool P_Cam9RTCPlayer::eventFilter(QObject *watched, QEvent *event) {
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


void P_Cam9RTCPlayer::updateSizeWhenExitFullScreenMode(){
    //    int topHeight = 40;
    //    int rightWidth = 420;
    qDebug()  << Q_FUNC_INFO << "updateSizeWhenExitFullScreenMode" << this->playerZone->size() <<this->zone->size();
    QSize sizeUpdate = this->zone->size();
    this->zone->resize(QSize(sizeUpdate.width() - 420 ,sizeUpdate.height() - 40));
    this->playerZone->move(0, 0);
    this->playerZone->updateGeometry();
}

void P_Cam9RTCPlayer::updateSizeWhenEnterFullScreenMode(){
    //    int topHeight = 40;
    //    int rightWidth = 420;
    qDebug()  << Q_FUNC_INFO << "updateSizeWhenExitFullScreenMode" << this->playerZone->size() <<this->zone->size();
    QSize sizeUpdate = this->zone->size();
    this->zone->resize(QSize(sizeUpdate.width() + 420 ,sizeUpdate.height() + 40));
    this->playerZone->move(0, 0);
    this->playerZone->updateGeometry();
}
