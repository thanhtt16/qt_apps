#include "p_profilesetting.h"

/**
     * Generic method to override for updating the presention.
     **/

P_ProfileSetting::P_ProfileSetting(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->hide();
    // init gui object
    this->zone->setStyleSheet("background-color: #FFFFFF");
    this->zone->hide();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 10, 0, 10);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignCenter);
    this->zone->setLayout(layout);

    QPushButton *settingButton = new QPushButton(this->zone);
    settingButton->setFixedSize(130, 30);
    settingButton->setIcon(QIcon(":/images/res/icon_tab_general.png"));
    settingButton->setIconSize(QSize(16, 16));
    settingButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    settingButton->setFont(Resources::instance().getLargeRegularButtonFont());
    settingButton->setStyleSheet(
                "background-color: #00000000 ; color: #2c3e50;border-top: "
                "0px;border-bottom:1px;border-left: 0px;border-left: "
                "none;border-right: 0px");
    settingButton->setText("Cài đặt");
    connect(settingButton, &QPushButton::clicked, this,
            &P_ProfileSetting::onSettingClicked);

    QPushButton *exitButton = new QPushButton(this->zone);
    exitButton->setFixedSize(130, 30);
    exitButton->setIcon(QIcon(":/images/res/icon_tab_exit.png"));
    exitButton->setIconSize(QSize(16, 16));
    exitButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    exitButton->setFont(Resources::instance().getLargeRegularButtonFont());
    exitButton->setStyleSheet(
                "background-color: #00000000 ; color: #2c3e50;border-top: "
                "0px;border-bottom:1px;border-left: 0px;border-left: "
                "none;border-right: 0px");
    exitButton->setText("Thoát");
    connect(exitButton, &QPushButton::clicked, this,
            &P_ProfileSetting::onExitButtonClicked);

    QPushButton *logoutButton = new QPushButton(this->zone);
    logoutButton->setFixedSize(130, 30);
    logoutButton->setIcon(QIcon(":/images/res/icon_tab_logout.png"));
    logoutButton->setIconSize(QSize(16, 16));
    logoutButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    logoutButton->setFont(Resources::instance().getLargeRegularButtonFont());
    connect(logoutButton, &QPushButton::clicked, this,
            &P_ProfileSetting::onLogoutButtonClicked);
    logoutButton->setText("Đăng xuất");
    logoutButton->setStyleSheet(
                "background-color: #00000000 ; color: #2c3e50;border-top: "
                "0px;border-bottom:1px;border-left: 0px;border-left: "
                "none;border-right: 0px");

    QPushButton *aboutButton = new QPushButton(this->zone);
    aboutButton->setFixedSize(130, 30);
    aboutButton->setIcon(QIcon(":/images/res/icon_about.png"));
    aboutButton->setIconSize(QSize(16, 16));
    aboutButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
    aboutButton->setFont(Resources::instance().getLargeRegularButtonFont());
    connect(aboutButton, &QPushButton::clicked, this,
            &P_ProfileSetting::onAboutButtonClicked);
    aboutButton->setText("Giới thiệu");
    aboutButton->setStyleSheet(
                "background-color: #00000000 ; color: #2c3e50;border-top: "
                "0px;border-bottom:1px;border-left: 0px;border-left: "
                "none;border-right: 0px");


    layout->addWidget(settingButton);
    layout->addWidget(aboutButton);
    layout->addWidget(exitButton);
    layout->addWidget(logoutButton);

    this->zone->setMouseTracking(true);
    this->zone->setAttribute(Qt::WA_Hover);
    this->zone->installEventFilter(this);
}

void P_ProfileSetting::onAboutButtonClicked(){
    control()->newUserAction(Message.SHOW_ABOUT_APP, Q_NULLPTR);
}

void P_ProfileSetting::update() {}

void P_ProfileSetting::show() {
    if (control()->zone->isVisible()) {
        control()->zone->hide();
    } else {
        control()->zone->show();
        control()->zone->raise();
    }
}

void P_ProfileSetting::onExitButtonClicked() {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(Q_NULLPTR);
    control()->newUserAction(Message.EXIT_APP, dataStruct);
}

void P_ProfileSetting::onLogoutButtonClicked() {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(Q_NULLPTR);
    control()->newUserAction(Message.LOGOUT, dataStruct);
}

void P_ProfileSetting::onSettingClicked() {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(0);
    control()->newUserAction(Message.APP_SHOW_SETTINGS, dataStruct);
}

void P_ProfileSetting::hide() { control()->zone->hide(); }

QWidget *P_ProfileSetting::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

bool P_ProfileSetting::eventFilter(QObject *watched, QEvent *event) {
    QWidget *zone = qobject_cast<QWidget *>(watched);
    if (zone == this->control()->zone) {
        switch (event->type()) {
        case QEvent::HoverEnter:
            hoverEnter(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverLeave:
            hoverLeave(static_cast<QHoverEvent *>(event));
            return true;
            break;
        case QEvent::HoverMove:
            hoverMove(static_cast<QHoverEvent *>(event));
            return true;
            break;
        default:
            break;
        }
    }

    //  return QWidget::eventFilter(watched, event);
    return false;
}

void P_ProfileSetting::hoverEnter(QHoverEvent *) {}

void P_ProfileSetting::hoverLeave(QHoverEvent *) {
    control()->newUserAction(Message.PROFILE_SETTING_HIDE_ALL, Q_NULLPTR);
    control()->zone->hide();
}

void P_ProfileSetting::hoverMove(QHoverEvent *) {}

void P_ProfileSetting::sizeTopControlBar(QVariant *dataStruct) {
    dataSizeTop = dataStruct->value<SizeTopControlBar>();
    int widthMove =
            dataSizeTop.widthTopBar - 25 - dataSizeTop.widthProfileSetting - 20;
    // init gui object
    this->zone->move(widthMove, 40);

    this->zone->setFixedSize(150, 150);
}
