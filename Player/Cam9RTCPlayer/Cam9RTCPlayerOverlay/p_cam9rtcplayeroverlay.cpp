#include "p_cam9rtcplayeroverlay.h"

/**
     * Generic method to override for updating the presention.
     **/

bool P_Cam9RTCPlayerOverlay::getIsLive() const { return isLive; }

void P_Cam9RTCPlayerOverlay::setIsLive(bool value) { isLive = value; }

P_Cam9RTCPlayerOverlay::P_Cam9RTCPlayerOverlay(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  // init gui object
  this->zone = zone;
  this->zone->installEventFilter(this);
  this->zone->setMouseTracking(true);

  this->zone->setStyleSheet(css.transCss /*css.coverCss*/);
  this->mainLayout = new QVBoxLayout();
  this->mainLayout->setMargin(5);
  this->mainLayout->setSpacing(0);
  this->zone->setLayout(this->mainLayout);

  // init top
  this->topBar = new QWidget(this->zone);
  this->mainLayout->addWidget(this->topBar);

  this->topBar->setStyleSheet(css.transCss);
  this->topLayout = new QHBoxLayout();
  this->topBar->setLayout(this->topLayout);

  this->topBar->setFixedHeight(20);
  this->topLayout->setAlignment(Qt::AlignTop);
  this->topLayout->setMargin(0);
  this->topLayout->setSpacing(5);

  // init top left
  QWidget *topLeftZone = new QWidget(this->topBar);
  this->topLayout->addWidget(topLeftZone);
  QHBoxLayout *topLeftLayout = new QHBoxLayout();
  topLeftLayout->setMargin(0);
  topLeftLayout->setSpacing(5);
  topLeftZone->setLayout(topLeftLayout);

  this->statusLabel = new QLabel(topLeftZone);
  this->statusLabel->setFixedSize(40, 20);
  this->statusLabel->setFont(Resources::instance().getSmallBoldButtonFont());
  this->statusLabel->setAlignment(Qt::AlignCenter);
  this->statusLabel->setStyleSheet(css.stoppedCss);
  this->statusLabel->setText(msg.live);
  topLeftLayout->addWidget(this->statusLabel);

  this->titleLabel = new QLabel(topLeftZone);
  this->titleLabel->setScaledContents(true);
  this->titleLabel->setFixedHeight(20);
  this->titleLabel->setStyleSheet(css.transCss);
  this->titleLabel->setFont(Resources::instance().getMediumBoldButtonFont());
  QGraphicsDropShadowEffect *titleEffect =
      new QGraphicsDropShadowEffect(this->zone);
  titleEffect->setBlurRadius(1);
  titleEffect->setColor(QColor(css.black));
  titleEffect->setOffset(1, 1);
  this->titleLabel->setGraphicsEffect(titleEffect);
  topLeftLayout->addWidget(this->titleLabel);

  // init top right
  QWidget *topRightZone = new QWidget(this->topBar);
  this->topLayout->addWidget(topRightZone);
  QHBoxLayout *topRightLayout = new QHBoxLayout();
  topRightLayout->setSpacing(1);
  topRightLayout->setMargin(0);
  topRightLayout->setSizeConstraint(QLayout::SetFixedSize);
  topRightZone->setLayout(topRightLayout);

  this->sdButton = new QPushButton(topRightZone);
  this->sdButton->setStyleSheet(css.leftSelectedButton);
  this->sdButton->setFixedSize(30, 20);
  this->sdButton->setText("SD");
  topRightLayout->addWidget(this->sdButton);
  connect(sdButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onSdClicked);

  this->hdButton = new QPushButton(topRightZone);
  this->hdButton->setStyleSheet(css.middleNormalButton);
  this->hdButton->setFixedSize(30, 20);
  this->hdButton->setText("HD");
  topRightLayout->addWidget(this->hdButton);
  connect(hdButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onHdClicked);

  this->hideFullscreenButton = new QPushButton(topRightZone);
  this->hideFullscreenButton->setStyleSheet(css.rightNormalButton);
  this->hideFullscreenButton->setFixedSize(30, 20);
  this->hideFullscreenButton->setText(msg.hideFullscreen);
  topRightLayout->addWidget(this->hideFullscreenButton);
  connect(this->hideFullscreenButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onHideFullscreenClicked);

  this->showFullscreenButton = new QPushButton(topRightZone);
  this->showFullscreenButton->setStyleSheet(css.rightNormalButton);
  this->showFullscreenButton->setFixedSize(30, 20);
  QPixmap pixMapFullScreen(":/images/res/fullscreen.png");
  QIcon iconFullSceen(pixMapFullScreen);
  this->showFullscreenButton->setIcon(iconFullSceen);

  //  this->showFullscreenButton->setText(msg.showFullscreen);
  //  QGraphicsDropShadowEffect *showEffect =
  //      new QGraphicsDropShadowEffect(this->zone);
  //  showEffect->setBlurRadius(1);
  //  showEffect->setColor(QColor(css.black));
  //  showEffect->setOffset(1, 1);
  //  this->showFullscreenButton->setGraphicsEffect(showEffect);
  topRightLayout->addWidget(this->showFullscreenButton);
  connect(this->showFullscreenButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onShowFullscreenClicked);

  // init center
  this->centerZone = new QWidget(this->zone);
  this->mainLayout->addWidget(this->centerZone);
  this->centerZone->setStyleSheet(css.transCss);
  this->centerLayout = new QVBoxLayout();
  this->centerLayout->setAlignment(Qt::AlignCenter);
  this->centerZone->setLayout(this->centerLayout);

  this->messageLabel = new QLabel(this->centerZone);
  this->messageLabel->setStyleSheet(css.border(css.transparent, css.white, 0, 5));
  this->messageLabel->setAlignment(Qt::AlignCenter);
  this->messageLabel->setFixedSize(200, 100);
  this->centerLayout->addWidget(this->messageLabel);
  this->messageLabel->setText("NO VIDEO");
  this->messageLabel->setFont(Resources::instance().getMediumBoldButtonFont());

  //  this->loaderMovie = new QMovie(":/images/res/loader.gif");
  //  this->loaderMovie->setSpeed(500);
  //  this->loaderMovie->start();
  //  this->messageLabel->setMovie(this->loaderMovie);

  // init bottom
  this->bottomBar = new QWidget(this->zone);
  this->mainLayout->addWidget(this->bottomBar);
  this->bottomBar->setStyleSheet(css.transCss);
  this->bottomBar->setFixedHeight(20);
  //  this->bottomBar->hide();

  this->bottomLayout = new QHBoxLayout();
  this->bottomLayout->setMargin(0);
  this->bottomLayout->setSpacing(5);
  this->bottomLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  this->bottomBar->setLayout(this->bottomLayout);

  this->normalRecordButton = new QPushButton(this->bottomBar);
  this->normalRecordButton->setFont(
      Resources::instance().getSmallBoldButtonFont());
  this->normalRecordButton->setText(msg.rec);
  this->normalRecordButton->setFixedSize(30, 20);
  this->normalRecordButton->setStyleSheet(
      css.border(css.white, css.playingLiveColor, 0, 3));
  this->bottomLayout->addWidget(this->normalRecordButton);
  connect(this->normalRecordButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onNormalRecordButtonClicked);

  this->quickRecordButton = new QPushButton(this->bottomBar);
  this->quickRecordButton->setFont(
      Resources::instance().getMediumRegularButtonFont());
  this->quickRecordButton->setText(msg.calendar);
  this->quickRecordButton->setFixedSize(30, 20);
  this->quickRecordButton->setStyleSheet(
      css.border(css.white, css.black, 0, 3));
  this->bottomLayout->addWidget(this->quickRecordButton);
  connect(this->quickRecordButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onQuickRecordButtonClicked);

  this->screenShootButton = new QPushButton(this->bottomBar);
  this->screenShootButton->setFont(
      Resources::instance().getMediumRegularButtonFont());
  this->screenShootButton->setText(msg.screenshot);
  this->screenShootButton->setFixedSize(30, 20);
  this->screenShootButton->setStyleSheet(
      css.border(css.white, css.black, 0, 3));
  this->bottomLayout->addWidget(this->screenShootButton);
  connect(this->screenShootButton, &QPushButton::clicked, this,
          &P_Cam9RTCPlayerOverlay::onScreenShootButtonClicked);

  // add widget to main
  this->mainLayout->addWidget(this->topBar);
  this->mainLayout->addWidget(this->centerZone);
  this->mainLayout->addWidget(this->bottomBar);

  // record
  animationNormalRecordButton();

  showFullscreenButton->show();
  hideFullscreenButton->hide();
  screenShootButton->hide();
  normalRecordButton->hide();
  quickRecordButton->hide();
  screenShootButton->hide();

  playerStopped();
}

void P_Cam9RTCPlayerOverlay::showLoader() {
//    this->zone->setStyleSheet("background-color: #80008000");
    this->messageLabel->show();
}

void P_Cam9RTCPlayerOverlay::hideLoader() {
//    this->zone->setStyleSheet("background-color: #80000000");
    this->messageLabel->hide();
}

void P_Cam9RTCPlayerOverlay::setState(PlayerState state) { Q_UNUSED(state) }

void P_Cam9RTCPlayerOverlay::setTitle(QString title) {
  this->titleLabel->setText(title);
}

void P_Cam9RTCPlayerOverlay::onSdClicked() {
  if (camItem != Q_NULLPTR) {
    this->sdButton->setStyleSheet(css.leftSelectedButton);
    this->hdButton->setStyleSheet(css.middleNormalButton);
    control()->newUserAction(Message.PLAYER_PLAY_LIVE_SD, Q_NULLPTR);
  }
}

void P_Cam9RTCPlayerOverlay::onHdClicked() {
  if (camItem != Q_NULLPTR) {
    this->sdButton->setStyleSheet(css.leftNormalButton);
    this->hdButton->setStyleSheet(css.middleSelectedButton);
    if (this->isLive) {
      control()->newUserAction(Message.PLAYER_PLAY_LIVE_HD, Q_NULLPTR);
    } else {
      control()->newUserAction(Message.PLAYER_PLAY_VOD_HD, Q_NULLPTR);
    }
  }
}

void P_Cam9RTCPlayerOverlay::onHideFullscreenClicked() {
  if (camItem != Q_NULLPTR) {
    isFullScreen = false;
    control()->newAction(Message.PLAYER_BEGIN_HIDE_FULLSCREEN, Q_NULLPTR);
    showFullscreenButton->show();
    hideFullscreenButton->hide();
    screenShootButton->hide();
    normalRecordButton->hide();
    quickRecordButton->hide();
    screenShootButton->hide();
  }
}

void P_Cam9RTCPlayerOverlay::onShowFullscreenClicked() {
  if (camItem != Q_NULLPTR) {
    isFullScreen = true;
    control()->newAction(Message.PLAYER_BEGIN_SHOW_FULLSCREEN, Q_NULLPTR);
    showFullscreenButton->hide();
    hideFullscreenButton->show();
    screenShootButton->show();
    if (!isLive) {
      normalRecordButton->show();
      quickRecordButton->show();
      screenShootButton->show();
    }
  }
}

///
/// \brief P_Cam9RTCPlayerOverlay::playerLoading
/// Hiển thị trạng thái giao diện khi đang tải dữ liệu.
/// Ẩn các thanh điều khiển hiển thị loading.
///
void P_Cam9RTCPlayerOverlay::playerLoading() {
  this->statusLabel->setStyleSheet(css.stoppedCss);
//  this->showControl();
//  this->showLoader();
}

///
/// \brief P_Cam9RTCPlayerOverlay::playerPlaying
/// Hiển thị trạng thái giao diện đang chạy.
/// Hiển thị thanh điều khiển và ẩn loading.
///
void P_Cam9RTCPlayerOverlay::playerPlaying() {
  this->statusLabel->setStyleSheet(this->isLive ? css.playingLiveCss
                                                : css.playingVodCss);
  this->statusLabel->setText(this->isLive ? msg.live : msg.vod);
//  this->showAll();
//  this->showControl();
//  this->hideLoader();
}

void P_Cam9RTCPlayerOverlay::playerPaused() {
  this->statusLabel->setStyleSheet(css.stoppedCss);
  this->hideLoader();
}

///
/// \brief P_Cam9RTCPlayerOverlay::playerStopped
/// Ẩn tất cả giao diện thanh điều khiển và loading
///

void P_Cam9RTCPlayerOverlay::clearStateLive(){
    this->statusLabel->setStyleSheet(css.stoppedCss);
}

void P_Cam9RTCPlayerOverlay::playerStopped() {
  this->statusLabel->setStyleSheet(css.stoppedCss);
  this->hideAll();
  this->hideControl();
  this->hideLoader();
}

void P_Cam9RTCPlayerOverlay::onNormalRecordButtonClicked() {
  QVariant *dataStruct = new QVariant();
  control()->newUserAction(Message.PLAYER_RECORD_NORMAL, dataStruct);
}

void P_Cam9RTCPlayerOverlay::onQuickRecordButtonClicked() {
  QVariant *dataStruct = new QVariant();
  control()->newUserAction(Message.PLAYER_RECORD_QUICK, dataStruct);
}

void P_Cam9RTCPlayerOverlay::onScreenShootButtonClicked() {
  QVariant *dataStruct = new QVariant();
  control()->newUserAction(Message.PLAYER_TAKE_SCREENSHOT, dataStruct);
}

///
/// \brief P_Cam9RTCPlayerOverlay::updateInfo
/// Cập nhật thông tin camera lên giao diện.
/// Nếu camera null thì ẩn tất cả giao diện.
/// Ngược lại thì hiển thị thông tin như trạng thái, địa điểm và các nút điều
/// khiển trên giao diện.
/// \param camItem
///
void P_Cam9RTCPlayerOverlay::updateInfo(CamItem *camItem) {
  this->camItem = camItem;
  if (this->camItem != Q_NULLPTR) {
    this->setTitle(camItem->getPostion());
    this->isLive = camItem->getIsLive();
    this->isMain = camItem->getIsMain();
    if (this->isMain) {
      displaySelectedHd();
    } else {
      displaySelectedSd();
    }
    if (this->isLive) {
      this->statusLabel->setText(msg.live);
    } else {
      this->statusLabel->setText(msg.vod);
    }
    this->showControl();
    this->showAll();
  } else {
    this->hideControl();
    this->hideAll();
  }
}

void P_Cam9RTCPlayerOverlay::displaySelectedSd() {
  this->isMain = false;
  this->sdButton->setStyleSheet(css.leftSelectedButton);
  this->hdButton->setStyleSheet(css.middleNormalButton);
}

void P_Cam9RTCPlayerOverlay::displaySelectedHd() {
  this->isMain = true;
  this->sdButton->setStyleSheet(css.leftNormalButton);
  this->hdButton->setStyleSheet(css.middleSelectedButton);
}

void P_Cam9RTCPlayerOverlay::update() {}

QWidget *P_Cam9RTCPlayerOverlay::getZone(int zoneId) {
  switch (zoneId) {
    case 0:
      return this->zone;
    default:
      return Q_NULLPTR;
  }
}

void P_Cam9RTCPlayerOverlay::animationNormalRecordButton() {
  // animaintion record
  pFadeEffect = new QGraphicsOpacityEffect(this->zone);
  normalRecordButton->setGraphicsEffect(pFadeEffect);
  pFadeEffect->setOpacity(1);
  pRecordVideoAnimation = new QPropertyAnimation(pFadeEffect, "opacity");
  pRecordVideoAnimation->setEasingCurve(QEasingCurve::SineCurve);
  pRecordVideoAnimation->setDuration(1000);
  pRecordVideoAnimation->setStartValue(0.1);
  pRecordVideoAnimation->setEndValue(1.0);
  pRecordVideoAnimation->setLoopCount(std::numeric_limits<quint64>::max());
}
void P_Cam9RTCPlayerOverlay::startAnimationNormalButton() {
  pRecordVideoAnimation->start();
}

void P_Cam9RTCPlayerOverlay::stopAnimationNormalButton() {
  pRecordVideoAnimation->stop();
  pFadeEffect->setOpacity(1);
}

bool P_Cam9RTCPlayerOverlay::eventFilter(QObject *watched, QEvent *event) {
  QWidget *sender = qobject_cast<QWidget *>(watched);
  if (sender != Q_NULLPTR && sender == this->zone) {
    switch (event->type()) {
      case QEvent::MouseButtonPress: {
        this->isMoving = true;
        QMouseEvent *mouseEvent = (QMouseEvent *)(event);
        this->pressPos = mouseEvent->pos();
        if (mouseEvent->button() == Qt::LeftButton) {
          this->onShowFullscreenClicked();
        } else if (mouseEvent->button() == Qt::RightButton) {
          if (isFullScreen == true) {
            this->onHideFullscreenClicked();
          }
        }
      } break;

      case QEvent::Wheel: {
        QWheelEvent *wheelEvent = (QWheelEvent *)(event);
        if (wheelEvent) {
          QPoint dp;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
          qreal deg = wheelEvent->angleDelta().y() / 8;
          dp = wheelEvent->pixelDelta();
#else
          qreal deg = wheelEvent->delta() / 8;
#endif  // QT_VERSION

          DataZoomVideo dataZoomVideo;
          dataZoomVideo.deg = deg;
          dataZoomVideo.pixelDelta = dp;
          dataZoomVideo.pos = wheelEvent->pos();

          QVariant *dataStruct = new QVariant();
          dataStruct->setValue<DataZoomVideo>(dataZoomVideo);
          control()->newUserAction(Message.WHEEL_EVENT_ZOOM_VIDEO, dataStruct);
          wheelEvent->accept();
        }
      } break;

      case QEvent::MouseMove: {
        QMouseEvent *mouseMoveEvent = (QMouseEvent *)(event);
        if (mouseMoveEvent && this->isMoving) {
          DataMoveVideo dataMove;
          dataMove.pressPos = this->pressPos;
          dataMove.pos = mouseMoveEvent->pos();
          this->pressPos = dataMove.pos;
          QVariant *dataStruct = new QVariant();
          dataStruct->setValue<DataMoveVideo>(dataMove);
          control()->newUserAction(Message.EVENT_MOVE_ZOOM_VIDEO, dataStruct);
        }
      } break;

      case QEvent::MouseButtonRelease: {
        this->isMoving = false;
      } break;

      default:
        break;
    }
  }
  return true;
}

void P_Cam9RTCPlayerOverlay::exitPopOutMode() {
  if (this->isLive) {
    hideFullscreenButton->hide();
    showFullscreenButton->show();
    screenShootButton->hide();
  } else {
    hideFullscreenButton->hide();
    showFullscreenButton->show();
    screenShootButton->hide();
    normalRecordButton->hide();
    quickRecordButton->hide();
  }
}

void P_Cam9RTCPlayerOverlay::hideAll() {
  this->zone->hide();
  this->zone->raise();
}

void P_Cam9RTCPlayerOverlay::showAll() {
  this->zone->show();
  this->zone->raise();
}

void P_Cam9RTCPlayerOverlay::hideControl() {
  this->topBar->hide();
  this->topBar->raise();
  this->bottomBar->hide();
  this->bottomBar->raise();
}

void P_Cam9RTCPlayerOverlay::showControl() {
  this->topBar->show();
  this->topBar->raise();
  this->bottomBar->show();
  this->bottomBar->raise();
}
