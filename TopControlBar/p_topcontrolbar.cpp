#include "p_topcontrolbar.h"

/**
     * Generic method to override for updating the presention.
     **/

P_TopControlBar::P_TopControlBar(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  this->zone = zone;
  mScreenSize = Resources::instance().getScreenSize();
  zone->setStyleSheet("background-color: #404244; color: #ddd");

  // devide left & right
  QHBoxLayout *twoPartsLayout = new QHBoxLayout();
  twoPartsLayout->setSpacing(0);
  twoPartsLayout->setMargin(0);
  zone->setLayout(twoPartsLayout);
  QWidget *leftZone = new QWidget(zone);
  QWidget *rightZone = new QWidget(zone);
  twoPartsLayout->addWidget(leftZone);
  twoPartsLayout->addWidget(rightZone);

  // setup left zone
  leftZone->setFixedWidth(mScreenSize.width() * 0.6);
  QHBoxLayout *leftZoneLayout = new QHBoxLayout();
  leftZoneLayout->setAlignment(Qt::AlignLeft);
  leftZoneLayout->setSpacing(20);
  leftZoneLayout->setContentsMargins(25, 0, 0, 0);
  leftZone->setLayout(leftZoneLayout);

  // setup left element
  brandNameZone = new QWidget(zone);
  //  brandNameZone->setFixedWidth(150);
  siteNameZone = new QWidget(zone);
  //  siteNameZone->setFixedWidth(250);

  appNameZone = new QWidget(zone);
  appNameZone->setFixedWidth(150);
  appNameZone->setStyleSheet("background-color:#404244");
  siteNameZone->setStyleSheet("background-color:#404244");
  QHBoxLayout *brandNameZoneLayout = new QHBoxLayout();
  brandNameZoneLayout->setAlignment(Qt::AlignLeft);
  brandNameZoneLayout->setSpacing(0);
  brandNameZoneLayout->setMargin(0);
  brandNameZone->setLayout(brandNameZoneLayout);

  QHBoxLayout *siteNameZoneLayout = new QHBoxLayout();
  siteNameZoneLayout->setAlignment(Qt::AlignLeft);
  siteNameZoneLayout->setSpacing(10);
  siteNameZoneLayout->setContentsMargins(15, 0, 15, 0);
  siteNameZone->setLayout(siteNameZoneLayout);

  QHBoxLayout *appNameZoneLayout = new QHBoxLayout();
  appNameZoneLayout->setAlignment(Qt::AlignLeft);
  appNameZoneLayout->setSpacing(10);
  appNameZoneLayout->setContentsMargins(15, 0, 15, 0);
  appNameZone->setLayout(appNameZoneLayout);

  leftZoneLayout->addWidget(brandNameZone);
  leftZoneLayout->addWidget(siteNameZone);
  leftZoneLayout->addWidget(appNameZone);

  brandLogo = new QLabel(brandNameZone);
  brandLogo->setFixedSize(75, 35);
  brandLogo->setScaledContents(true);
  loadLogoApp();
  // load logo

  QLabel *brandName = new QLabel(brandNameZone);
  brandName->setFont(Resources::instance().getExtraLargeRegularButtonFont());

  // site icon
  this->siteIcon = new QLabel(siteNameZone);
  this->siteIcon->setFont(
      Resources::instance().getExtraLargeRegularButtonFont());
  this->siteIcon->setText("");

  // site name
  this->siteName = new QPushButton(siteNameZone);
  this->siteName->setStyleSheet(
      "background-color: #00000000; color: #FFFFFF;border: none;");
  this->siteName->setFont(
      Resources::instance().getExtraLargeRegularButtonFont());

  // app icon
  this->appIcon = new QLabel(appNameZone);
  this->appIcon->setFont(
      Resources::instance().getExtraLargeRegularButtonFont());
  this->appName = new QPushButton(appNameZone);
  this->appName->setFont(
      Resources::instance().getExtraLargeRegularButtonFont());
  this->appName->setStyleSheet(
      "background-color: #00000000 ; color: #FFFFFF;border: none;");
  brandNameZoneLayout->addWidget(brandLogo);
  brandNameZoneLayout->addWidget(brandName);

  siteNameZoneLayout->addWidget(siteIcon);
  siteNameZoneLayout->addWidget(siteName);

  appNameZoneLayout->addWidget(appIcon);
  appNameZoneLayout->addWidget(appName);

  QString brandNameString =
      AppProfile::getAppProfile()->getAppConfig()->getBrandName();
  QString first = brandNameString.mid(0, 1);
  QString second = brandNameString.mid(1);
  brandName->setText(brandNameString);
  siteName->setText(control()->appContext->getWorkingSite()->getSiteName());
  UserApp appWorking = control()->appContext->getWorkingApp();
  appIcon->setText(appWorking.appIcon);
  appName->setText(appWorking.appName);

  // setup right zone
  rightZone->setFixedWidth(mScreenSize.width() * 0.4);
  QHBoxLayout *rightZoneLayout = new QHBoxLayout();
  rightZoneLayout->setAlignment(Qt::AlignRight);
  rightZoneLayout->setSpacing(20);
  rightZoneLayout->setContentsMargins(0, 0, 25, 0);
  rightZone->setLayout(rightZoneLayout);

  // init full screen

  fullScreenWidget = new QWidget(rightZone);
  fullScreenWidget->setStyleSheet("background-color:#404244");
  fullScreenWidget->setFixedWidth(60);
  rightZoneLayout->addWidget(fullScreenWidget);

  btnFullScreen = new QPushButton(fullScreenWidget);
  connect(btnFullScreen, &QPushButton::clicked, this,
          &P_TopControlBar::clickFullSreen);
  btnFullScreen->setStyleSheet(
      "background-color: #00000000 ; color: #FFFFFF; border: none;");
  btnFullScreen->setText("");
  QHBoxLayout *fullScreenLayout = new QHBoxLayout();
  fullScreenLayout->setSpacing(10);
  fullScreenLayout->setMargin(0);
  fullScreenWidget->setLayout(fullScreenLayout);
  fullScreenLayout->addWidget(btnFullScreen);

  profileSettingWidget = new QWidget(rightZone);
  profileSettingWidget->setFixedWidth(150);
  profileSettingWidget->setStyleSheet("background-color:#404244");
  rightZoneLayout->addWidget(profileSettingWidget);

  QHBoxLayout *profileSettingLayout = new QHBoxLayout();
  profileSettingLayout->setSpacing(10);
  profileSettingLayout->setMargin(10);
  profileSettingWidget->setLayout(profileSettingLayout);

  profileSetting = new QPushButton(profileSettingWidget);
  profileSetting->setStyleSheet(
      "background-color: #00000000 ; color: #FFFFFF; border: none;");
  profileSettingLayout->addWidget(profileSetting);
  profileSetting->setFont(
      Resources::instance().getExtraLargeRegularButtonFont());
  profileSetting->setText("Chức năng");

  // connect action left
  connect(siteName, &QPushButton::clicked, this,
          &P_TopControlBar::onSiteNameClick);
  connect(appName, &QPushButton::clicked, this,
          &P_TopControlBar::onAppNameClick);
  connect(profileSetting, &QPushButton::clicked, this,
          &P_TopControlBar::onProfileSettingClick);

  siteSelectorZone = new QWidget(control()->appContext->getMainWindow());
  appSelectorZone = new QWidget(control()->appContext->getMainWindow());
  profileSettingZone = new QWidget(control()->appContext->getMainWindow());

  appNameZone->setMouseTracking(true);
  appNameZone->setAttribute(Qt::WA_Hover);
  appNameZone->installEventFilter(this);

  siteNameZone->setMouseTracking(true);
  siteNameZone->setAttribute(Qt::WA_Hover);
  siteNameZone->installEventFilter(this);

  profileSettingWidget->setMouseTracking(true);
  profileSettingWidget->setAttribute(Qt::WA_Hover);
  profileSettingWidget->installEventFilter(this);
  qDebug() << "P_TopControlBar Success";
}

// action left element
void P_TopControlBar::onSiteNameClick() {
  control()->newUserAction(Message.SITE_SELECTOR_SHOW, Q_NULLPTR);
}

void P_TopControlBar::onAppNameClick() {
  control()->newUserAction(Message.APP_SELECTOR_SHOW, Q_NULLPTR);
  //  appName->setStyleSheet("background:#e74c3c;color:#FFFFFF");
}

void P_TopControlBar::onProfileSettingClick() {
  control()->newUserAction(Message.PROFILE_SETTING_SHOW, Q_NULLPTR);
}

// action right element
void P_TopControlBar::onPageControlClick() {}

void P_TopControlBar::onLayoutControlClick() {}

void P_TopControlBar::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_TopControlBar::hide() {}

void P_TopControlBar::update() {}

QWidget *P_TopControlBar::getZone(int zoneId) {
  switch (zoneId) {
    case Message.SITE_SELECTOR:
      return siteSelectorZone;
    case Message.APP_SELECTOR:
      return appSelectorZone;
    case Message.PROFILE_SETTING:
      return profileSettingZone;
    default:
      return Q_NULLPTR;
  }
}

void P_TopControlBar::loadLogoApp() {
  QString folder = QDir::homePath();
  folder = folder.append("/.videowall/images/logo");
  if (!QDir(folder).exists()) {
    QDir().mkpath(folder);
  }
  QString path = folder + QString("/logo.png");
  QPixmap pixmap(path);

  brandLogo->setPixmap(pixmap);
}

void P_TopControlBar::siteChanged() {
  this->siteIcon->setText("");
  this->siteName->setText(
      control()->appContext->getWorkingSite()->getSiteName());
}


void P_TopControlBar::enableSelectedApp() {
  siteName->setEnabled(true);
  appName->setEnabled(true);
}
void P_TopControlBar::disableSelectedApp() {
  siteName->setEnabled(false);
  appName->setEnabled(false);
}
void P_TopControlBar::appChanged() {
  this->appIcon->setText(control()->appContext->getWorkingApp().appIcon);
  this->appName->setText(control()->appContext->getWorkingApp().appName);
}

bool P_TopControlBar::eventFilter(QObject *watched, QEvent *event) {
  QWidget *widget = qobject_cast<QWidget *>(watched);

  if (widget == siteNameZone || widget == appNameZone ||
      widget == profileSettingWidget) {
    switch (event->type()) {
      case QEvent::HoverEnter: {
        if (widget == appNameZone) {
          appNameZone->setStyleSheet("background:#445a4d;color:#FFFFFF");
          siteNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
          profileSettingWidget->setStyleSheet(
              "background:#404244;color:#FFFFFF");
          control()->newUserAction(Message.APP_SELECTOR_SHOW, Q_NULLPTR);
          control()->newUserAction(Message.SITE_SELECTOR_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.PROFILE_SETTING_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.TOP_BAR_OVER_LAY_CAN_NOT_HIDE,
                                   Q_NULLPTR);
        } else if (widget == siteNameZone) {
          siteNameZone->setStyleSheet("background:#445a4d;color:#FFFFFF");
          appNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
          profileSettingWidget->setStyleSheet(
              "background:#404244;color:#FFFFFF");
          control()->newUserAction(Message.APP_SELECTOR_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.SITE_SELECTOR_SHOW, Q_NULLPTR);
          control()->newUserAction(Message.PROFILE_SETTING_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.TOP_BAR_OVER_LAY_CAN_NOT_HIDE,
                                   Q_NULLPTR);
        } else if (widget == profileSettingWidget) {
          profileSettingWidget->setStyleSheet(
              "background:#445a4d;color:#FFFFFF");
          siteNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
          appNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
          control()->newUserAction(Message.APP_SELECTOR_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.SITE_SELECTOR_HIDE, Q_NULLPTR);
          control()->newUserAction(Message.PROFILE_SETTING_SHOW, Q_NULLPTR);
          control()->newUserAction(Message.TOP_BAR_OVER_LAY_CAN_NOT_HIDE,
                                   Q_NULLPTR);
        }
        return true;
      } break;

      case QEvent::HoverLeave: {
        return true;
      } break;

      case QEvent::HoverMove: {
      } break;

      case QEvent::Resize: {
        if (widget == siteNameZone) {
          QVariant *dataStruct = new QVariant();
          SizeTopControlBar dataSize;
          dataSize.widthBrand = brandNameZone->width();
          dataSize.widthSite = siteNameZone->width();
          dataSize.widthAppName = appNameZone->width();
          dataSize.widthProfileSetting = profileSetting->width();
          dataSize.widthTopBar = this->zone->width();
          dataStruct->setValue<SizeTopControlBar>(dataSize);
          control()->newUserAction(Message.GET_SIZE_TOP_CONTROL_BAR,
                                   dataStruct);
        }
      } break;

      default:
        break;
    }
  }

  //  return QWidget::eventFilter(watched, event);
  return false;
}

void P_TopControlBar::hideElementTop() {
  appNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
  siteNameZone->setStyleSheet("background:#404244;color:#FFFFFF");
  profileSettingWidget->setStyleSheet("background:#404244;color:#FFFFFF");
}

// void P_TopControlBar::enterFullscreenMode() {
//}

// void P_TopControlBar::exitFullscreenMode() {
//  rightBar->setFixedWidth(appSize.rightWidth);
//}

void P_TopControlBar::clickFullSreen() {
  if (isButtonFullcreenClick == false) {
    isButtonFullcreenClick = true;
    control()->getParent()->newAction(Message.EXIT_FULLSCREEN_MODE, Q_NULLPTR);
    fullScreenWidget->setStyleSheet("background:#445a4d;color:#FFFFFF");

  } else {
    fullScreenWidget->setStyleSheet("background:#404244;color:#FFFFFF");
    isButtonFullcreenClick = false;
    control()->getParent()->newAction(Message.ENTER_FULLSCREEN_MODE, Q_NULLPTR);
    control()->getParent()->newAction(Message.TOP_BAR_OVER_LAY_CAN_HIDE,
                                      Q_NULLPTR);
  }
}
