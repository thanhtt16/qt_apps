#include "appsettingwidget.h"

AppSettingWidget::AppSettingWidget(QWidget *parent) : QWidget(parent) {
  initUI();
}

void AppSettingWidget::initUI() {
  setFixedSize(1280, 720);
  pMainLayout = new QHBoxLayout();
  pMainLayout->setMargin(0);
  pMainLayout->setSpacing(0);
  pMainLayout->setAlignment(Qt::AlignLeft);
  setLayout(pMainLayout);

  initLeftSide();
  initRightSide();
}

void AppSettingWidget::initLeftSide() {
  pLeftWidget = new QWidget(this);
  pLeftWidget->setFixedSize(360, 720);
  //    pLeftWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  pMainLayout->addWidget(pLeftWidget);

  pLeftLayout = new QVBoxLayout();
  pLeftLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  pLeftLayout->setMargin(5);
  pLeftLayout->setSpacing(5);
  pLeftWidget->setLayout(pLeftLayout);

  pLeftTopWidget = new QWidget(pLeftWidget);
  pLeftTopLayout = new QHBoxLayout();
  pLeftTopLayout->setMargin(0);
  pLeftTopLayout->setSpacing(5);
  pLeftTopWidget->setLayout(pLeftTopLayout);
  pLeftLayout->addWidget(pLeftTopWidget);

  pNameListModel = new QStringListModel();

  pNameListView = new QListView(pLeftWidget);
  pNameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  pNameListView->setFont(Resources::instance().getLargeRegularButtonFont());
  pNameListView->setModel(pNameListModel);
  connect(pNameListView, &QListView::clicked, this,
          &AppSettingWidget::onSelectedProfileAt);
  pLeftLayout->addWidget(pNameListView);

  QStringList names;
  for (int index = 0; index < AppProfile::appProfiles->size(); ++index) {
    AppProfile *appProfile = AppProfile::appProfiles->at(index);
    names.append(appProfile->getName());
  }
  pNameListModel->setStringList(names);

  pBotLeftWidget = new QWidget(pLeftWidget);
  pBotLeftLayout = new QHBoxLayout();
  pBotLeftLayout->setMargin(0);
  pBotLeftLayout->setSpacing(10);
  pBotLeftLayout->setAlignment(Qt::AlignCenter);
  pBotLeftWidget->setLayout(pBotLeftLayout);
  pLeftLayout->addWidget(pBotLeftWidget);

  pAddButton = new QPushButton(pBotLeftWidget);
  pAddButton->setFixedSize(100, 30);
  //  pAddButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.EMERALD, colors.WHITE, 0, 5));
  pAddButton->setFont(Resources::instance().getLargeBoldButtonFont());
  pAddButton->setText("Thêm mới");
  pBotLeftLayout->addWidget(pAddButton);
  connect(pAddButton, &QPushButton::clicked, this,
          &AppSettingWidget::onAddClicked);

  pDeleteButton = new QPushButton(pBotLeftWidget);
  pDeleteButton->setFixedSize(100, 30);
  pDeleteButton->setFont(Resources::instance().getLargeBoldButtonFont());
  pDeleteButton->setText("Xóa");
  pBotLeftLayout->addWidget(pDeleteButton);
  connect(pDeleteButton, &QPushButton::clicked, this,
          &AppSettingWidget::onDeleteClicked);

  pApplyButton = new QPushButton(pBotLeftWidget);
  pApplyButton->setFixedSize(100, 30);
  pApplyButton->setFont(Resources::instance().getLargeBoldButtonFont());
  pApplyButton->setText("Áp dụng");
  pBotLeftLayout->addWidget(pApplyButton);
  connect(pApplyButton, &QPushButton::clicked, this,
          &AppSettingWidget::onApplyClicked);
}

void AppSettingWidget::onSelectedProfileAt(QModelIndex modelIndex) {
  if (modelIndex.row() >= 0) {
    mIsChanged = false;
    mLastSelectedIndex = mCurrentSelectedIndex;
    mCurrentSelectedIndex = modelIndex.row();
    pCurrentProfile =
        AppProfile::appProfiles->at(mCurrentSelectedIndex)->cloneAppProfile();
    displayCurrentProfile();
  }
}

void AppSettingWidget::displayCurrentProfile() {
  if (pCurrentProfile != Q_NULLPTR) {
    pNameLineEdit->setText(pCurrentProfile->getName());
    AppConfig *appConfig = pCurrentProfile->getAppConfig();
    pPingHostLineEdit->setText(appConfig->getPingHost());
    pBaseApiUrlLineEdit->setText(appConfig->getBaseApiUrl());
    pBaseRecordApiUrlLineEdit->setText(appConfig->getBaseRecordApiUrl());
    pBaseVodApiUrlLineEdit->setText(appConfig->getBaseVodApiUrl());
    pTokenUriLineEdit->setText(appConfig->getTokenApiUri());
    pLoginUriLineEdit->setText(appConfig->getLoginApiUri());
    pRecordUriLineEdit->setText(appConfig->getRecordApiUri());
    pCamSiteByDeviceUriLineEdit->setText(appConfig->getCamSiteByDeviceApiUri());
    pCamSiteBySiteUriLineEdit->setText(appConfig->getCamSiteBySiteApiUri());
  }
}

void AppSettingWidget::initRightSide() {
  pRightWidget = new QWidget(this);
  pRightWidget->setFixedSize(920, 720);
  pRightLayout = new QVBoxLayout();
  pRightLayout->setMargin(0);
  pRightLayout->setSpacing(10);
  pRightWidget->setLayout(pRightLayout);
  pMainLayout->addWidget(pRightWidget);

  initRightTopSide();
  initRightBottoomSide();
}

void AppSettingWidget::initRightTopSide() {
  pRightTopWidget = new QWidget(pRightWidget);
  pRightLayout->addWidget(pRightTopWidget);
  pRightTopWidget->setFixedSize(920, 680);

  pRightTopLayout = new QVBoxLayout();
  pRightTopLayout->setContentsMargins(0, 5, 5, 0);
  pRightTopLayout->setSpacing(5);
  pRightTopLayout->setAlignment(Qt::AlignCenter);
  pRightTopWidget->setLayout(pRightTopLayout);

  pConfigTabWidget = new QTabWidget(pRightTopWidget);
  pRightTopLayout->addWidget(pConfigTabWidget);

  initNetworkWidget();
  initLayoutWidget();
}

void AppSettingWidget::initNetworkWidget() {
  pNetworkWidget = new QWidget(pConfigTabWidget);
  pConfigTabWidget->addTab(
      pNetworkWidget,
      Resources::instance().getAwesome()->icon(fa::signal).pixmap(32, 32),
      "Network");

  pNetworkLayout = new QGridLayout();
  pNetworkLayout->setMargin(10);
  pNetworkLayout->setSpacing(10);
  pNetworkLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  pNetworkWidget->setLayout(pNetworkLayout);

  pPingHostLabel = createLabel(pNetworkWidget, "Host Ping:");
  pNetworkLayout->addWidget(pPingHostLabel, 0, 0);

  pPingHostLineEdit = createLineEdit(pNetworkWidget, "http://www.google.com");
  pNetworkLayout->addWidget(pPingHostLineEdit, 0, 1);

  pBaseApiUrlLabel = createLabel(pNetworkWidget, "Base Api Url:");
  pNetworkLayout->addWidget(pBaseApiUrlLabel, 1, 0);

  pBaseApiUrlLineEdit =
      createLineEdit(pNetworkWidget, "http://... or https://...");
  pNetworkLayout->addWidget(pBaseApiUrlLineEdit, 1, 1);

  pBaseRecordApiUrlLabel = createLabel(pNetworkWidget, "Base Record Api Url:");
  pNetworkLayout->addWidget(pBaseRecordApiUrlLabel, 2, 0);

  pBaseRecordApiUrlLineEdit =
      createLineEdit(pNetworkWidget, "http://... or https://...");
  pNetworkLayout->addWidget(pBaseRecordApiUrlLineEdit, 2, 1);

  pBaseVodApiUrlLabel = createLabel(pNetworkWidget, "Base Vod Api Url:");
  pNetworkLayout->addWidget(pBaseVodApiUrlLabel, 3, 0);

  pBaseVodApiUrlLineEdit =
      createLineEdit(pNetworkWidget, "http://... or https://...");
  pNetworkLayout->addWidget(pBaseVodApiUrlLineEdit, 3, 1);

  pLoginUriLabel = createLabel(pNetworkWidget, "Login Uri:");
  pNetworkLayout->addWidget(pLoginUriLabel, 4, 0);

  pLoginUriLineEdit =
      createLineEdit(pNetworkWidget, "Please enter login uri...");
  pNetworkLayout->addWidget(pLoginUriLineEdit, 4, 1);

  pTokenUriLabel = createLabel(pNetworkWidget, "Token Uri:");
  pNetworkLayout->addWidget(pTokenUriLabel, 5, 0);

  pTokenUriLineEdit =
      createLineEdit(pNetworkWidget, "Please enter token uri...");
  pNetworkLayout->addWidget(pTokenUriLineEdit, 5, 1);

  pRecordUriLabel = createLabel(pNetworkWidget, "Record Uri:");
  pNetworkLayout->addWidget(pRecordUriLabel, 6, 0);

  pRecordUriLineEdit =
      createLineEdit(pNetworkWidget, "Please enter record uri...");
  pNetworkLayout->addWidget(pRecordUriLineEdit, 6, 1);

  pCamSiteByDeviceUriLabel =
      createLabel(pNetworkWidget, "Cam Site by Device Id Uri:");
  pNetworkLayout->addWidget(pCamSiteByDeviceUriLabel, 7, 0);

  pCamSiteByDeviceUriLineEdit =
      createLineEdit(pNetworkWidget, "Please enter cam site by device uri...");
  pNetworkLayout->addWidget(pCamSiteByDeviceUriLineEdit, 7, 1);

  pCamSiteBySiteUriLabel =
      createLabel(pNetworkWidget, "Cam Site by Site Id Uri:");
  pNetworkLayout->addWidget(pCamSiteBySiteUriLabel, 8, 0);

  pCamSiteBySiteUriLineEdit =
      createLineEdit(pNetworkWidget, "Please enter cam site by site uri...");
  pNetworkLayout->addWidget(pCamSiteBySiteUriLineEdit, 8, 1);
}

QLabel *AppSettingWidget::createLabel(QWidget *parent, QString text) {
  QLabel *label = new QLabel(parent);
  label->setText(text);
  label->setFont(Resources::instance().getLargeBoldButtonFont());
  return label;
}

QLineEdit *AppSettingWidget::createLineEdit(QWidget *parent, QString text) {
  QLineEdit *lineEdit = new QLineEdit(parent);
  lineEdit->setFont(Resources::instance().getLargeRegularButtonFont());
  lineEdit->setPlaceholderText(text);
  //    lineEdit->setStyleSheet(utils.borderStyleSheet(colors.WHITE,
  //    colors.MIDNIGHT_BLUE, 0, 5));
  lineEdit->setFixedHeight(30);
  return lineEdit;
}

QSpinBox *AppSettingWidget::createSpinBox(QWidget *parent, int minValue,
                                          int maxValue, int stepValue,
                                          int initValue, QString suffix) {
  QSpinBox *spinBox = new QSpinBox(parent);
  spinBox->setFixedHeight(30);
  spinBox->setMinimum(minValue);
  spinBox->setMaximum(maxValue);
  spinBox->setValue(initValue);
  spinBox->setSingleStep(stepValue);
  spinBox->setSuffix(suffix);
  spinBox->setFont(Resources::instance().getLargeRegularButtonFont());
  //    spinBox->setStyleSheet(utils.borderStyleSheet(colors.WHITE,
  //    colors.MIDNIGHT_BLUE, 0, 5));
  return spinBox;
}

void AppSettingWidget::initLayoutWidget() {
  pLayoutWidget = new QWidget(pConfigTabWidget);
  pConfigTabWidget->addTab(
      pLayoutWidget,
      Resources::instance().getAwesome()->icon(fa::laptop).pixmap(32, 32),
      "Layout");
  //    pLayoutWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));

  pLayoutLayout = new QGridLayout();
  pLayoutLayout->setMargin(10);
  pLayoutLayout->setSpacing(10);
  pLayoutLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
  pLayoutWidget->setLayout(pLayoutLayout);

  pMaxLayoutLabel = createLabel(pLayoutWidget, "Max layout instance:");
  pLayoutLayout->addWidget(pMaxLayoutLabel, 0, 0);

  pMaxLayoutSpinBox = createSpinBox(pLayoutWidget, 1, 100, 1, 16, "  Videos");
  pLayoutLayout->addWidget(pMaxLayoutSpinBox, 0, 1);

  pSlowFpsLabel = createLabel(pLayoutWidget, "Slow Fps:");
  pLayoutLayout->addWidget(pSlowFpsLabel, 1, 0);

  pSlowFpsSpinBox = createSpinBox(pLayoutWidget, 1, 30, 1, 8, "  Fps");
  pLayoutLayout->addWidget(pSlowFpsSpinBox, 1, 1);

  pStopFpsLabel = createLabel(pLayoutWidget, "Stop Fps:");
  pLayoutLayout->addWidget(pStopFpsLabel, 2, 0);

  pStopFpsSpinBox = createSpinBox(pLayoutWidget, 1, 30, 1, 4, "  Fps");
  pLayoutLayout->addWidget(pStopFpsSpinBox, 2, 1);

  pWaitReplaySecondsLabel = createLabel(pLayoutWidget, "Wait time to replay:");
  pLayoutLayout->addWidget(pWaitReplaySecondsLabel, 3, 0);

  pWaitReplaySecondsSpinBox =
      createSpinBox(pLayoutWidget, 1, 30, 1, 4, "  seconds");
  pLayoutLayout->addWidget(pWaitReplaySecondsSpinBox, 3, 1);
}

void AppSettingWidget::initRightBottoomSide() {
  pRightBotWidget = new QWidget(pRightWidget);
  pRightLayout->addWidget(pRightBotWidget);
  //    pRightBotWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,
  //    colors.MIDNIGHT_BLUE));
  pRightBotWidget->setFixedSize(pRightWidget->width(), 40);
  pRightBotLayout = new QHBoxLayout();
  pRightBotLayout->setMargin(5);
  pRightBotLayout->setSpacing(10);
  pRightBotLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  pRightBotWidget->setLayout(pRightBotLayout);

  pNameLineEdit = new QLineEdit(pRightBotWidget);
  pNameLineEdit->setPlaceholderText("Tên dự cấu hình...");
  pNameLineEdit->setFixedHeight(30);
  //  pNameLineEdit->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 5));
  pNameLineEdit->setFont(Resources::instance().getLargeBoldButtonFont());
  pRightBotLayout->addWidget(pNameLineEdit);

  pSaveButton = new QPushButton(pRightBotWidget);
  pSaveButton->setFixedSize(100, 30);
  //  pSaveButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.EMERALD, colors.WHITE, 0, 5));
  pSaveButton->setFont(Resources::instance().getLargeBoldButtonFont());
  pSaveButton->setText("Lưu");
  pRightBotLayout->addWidget(pSaveButton);
  connect(pSaveButton, &QPushButton::clicked, this,
          &AppSettingWidget::onSaveButtonClicked);

  pCancelButton = new QPushButton(pRightBotWidget);
  pCancelButton->setFixedSize(100, 30);
  //  pCancelButton->setStyleSheet(
  //      utils.borderStyleSheet(colors.ALIZARIN, colors.WHITE, 0, 5));
  pCancelButton->setFont(Resources::instance().getLargeBoldButtonFont());
  pCancelButton->setText("Khôi phục");
  pRightBotLayout->addWidget(pCancelButton);
  connect(pCancelButton, &QPushButton::clicked, this,
          &AppSettingWidget::onCancelButtonClicked);
}

void AppSettingWidget::onAddClicked() {}

void AppSettingWidget::onApplyClicked() {}

void AppSettingWidget::onDeleteClicked() {}

void AppSettingWidget::onSaveButtonClicked() {
  pCurrentProfile->setName(pNameLineEdit->text());
  AppConfig *appConfig = pCurrentProfile->getAppConfig();
  appConfig->setPingHost(pPingHostLineEdit->text());
  appConfig->setBaseApiUrl(pBaseApiUrlLineEdit->text());
  appConfig->setBaseVodApiUrl(pBaseVodApiUrlLineEdit->text());
  appConfig->setBaseRecordApiUrl(pBaseRecordApiUrlLineEdit->text());
  appConfig->setLoginApiUri(pLoginUriLineEdit->text());
  appConfig->setTokenApiUri(pTokenUriLineEdit->text());
  appConfig->setRecordApiUri(pRecordUriLineEdit->text());
  appConfig->setCamSiteByDeviceApiUri(pCamSiteByDeviceUriLineEdit->text());
  appConfig->setCamSiteBySiteApiUri(pCamSiteBySiteUriLineEdit->text());

  int index = AppProfile::isExist(pCurrentProfile);
  if (index >= 0) {
    AppProfile::appProfiles->replace(index, pCurrentProfile);
  } else {
    AppProfile::appProfiles->append(pCurrentProfile);
  }

  AppProfile::saveAppProfiles(AppProfile::appProfiles);
}

void AppSettingWidget::onCancelButtonClicked() {
  if (!mIsChanged) {
  } else {
    pCurrentProfile->deleteLater();
    pCurrentProfile =
        AppProfile::appProfiles->at(mCurrentSelectedIndex)->cloneAppProfile();
    displayCurrentProfile();
  }
}

AppSettingWidget::~AppSettingWidget() {}
