#include "p_setting_general.h"

/**
     * Generic method to override for updating the presention.
     **/

P_SettingGeneral::P_SettingGeneral(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->setStyleSheet("color:#4c4b52");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->zone->setLayout(mainLayout);
    QWidget *gridWidget = new QWidget(this->zone);
    mainLayout->addWidget(gridWidget);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QGridLayout *gridLayout = new QGridLayout();
    gridWidget->setLayout(gridLayout);

    //site current
    siteWorkingLabel = new QLabel(gridWidget);
    siteWorkingLabel->setText("Site hiện tại : ");

    QSettings settings;
    QString networdTypeDefault = control()->appContext->getNetworkType().network;
    QString networkSaved = settings.value("network_type").toString();
    qDebug() <<Q_FUNC_INFO<< "SETTING" <<networdTypeDefault << networkSaved;

    settings.beginGroup(QString::number(control()->getParent()->getAppContext()->getWorkingUser()->getUserId()));
    QString nameSite = settings.value("site_name").toString();
    //    QString appVersion = settings.value("app_version").toString();
    QString appVersion = AppProfile::getAppProfile()->getAppConfig()->getAppVersion();

    workingSiteLineEdit = new QLineEdit(gridWidget);
    workingSiteLineEdit->setFont(
                Resources::instance().getLargeRegularButtonFont());
    workingSiteLineEdit->setText(nameSite);

    gridLayout->addWidget(siteWorkingLabel,0,0);
    gridLayout->addWidget(workingSiteLineEdit,0,1);

    //app version current

    //    appVersionLabel = new QLabel(gridWidget);
    //    appVersionLabel->setText("Phiên bản hiện tại: ");
    //    gridLayout->addWidget(appVersionLabel, 1,0);
    //    versionLineEdit = new QLineEdit(gridWidget);
    //    versionLineEdit->setText("Videowall " + appVersion);
    //    gridLayout->addWidget(versionLineEdit,1,1);

    streamLabel = new QLabel(gridWidget);
    streamLabel->setText("Chọn luồng : ");
    gridLayout->addWidget(streamLabel, 1,0);
    //select protocol
    QGroupBox *groupBoxStream = new QGroupBox();
    groupBoxStream->setMaximumWidth(300);
    cdnRadioButton = new QRadioButton(tr("CDN"));
    nvrRadioButton = new QRadioButton(tr("LOCAL"));
    QHBoxLayout *layoutStream = new QHBoxLayout();
    layoutStream->setContentsMargins(30,0,20,0);
    if(networkSaved == "CDN" || networkSaved == "LAN") {networdTypeDefault = networkSaved;}

    if(networdTypeDefault == "CDN"){
        cdnRadioButton->setChecked(true);
    }else if(networdTypeDefault == "LAN"){
        nvrRadioButton->setChecked(true);
    }
    groupBoxStream->setLayout(layoutStream);
    layoutStream->addWidget(nvrRadioButton);
    layoutStream->addWidget(cdnRadioButton);
    gridLayout->addWidget(groupBoxStream,1,1);
    connect(cdnRadioButton, &QRadioButton::clicked, this, &P_SettingGeneral::cdnSelected);
    connect(nvrRadioButton, &QRadioButton::clicked, this, &P_SettingGeneral::nvrSelected);

}

void P_SettingGeneral::cdnSelected(bool checked){
    if(checked){
        control()->appContext->getNetworkType().network = "CDN";
        CamItemType type;
        type.network = "CDN";
        type.protocol = "WS";
        QSettings settings;
        settings.setValue("protocol_type",type.protocol);
        settings.setValue("network_type", type.network);

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<CamItemType>(type);
        control()->newUserAction(Message.UPDATE_CDN_TYPE_SELECTED, dataStruct);
    }
}

void P_SettingGeneral::nvrSelected(bool checked){
    if(checked){
        control()->appContext->getNetworkType().network = "LAN";
        CamItemType type;
        type.network = "LAN";
        type.protocol = "WS";
        QSettings settings;
        settings.setValue("protocol_type",type.protocol);
        settings.setValue("network_type", type.network);

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<CamItemType>(type);
        control()->newUserAction(Message.UPDATE_CDN_TYPE_SELECTED, dataStruct);
    }
}
QObject *P_SettingGeneral::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}
void P_SettingGeneral::updateWorkingSite(Site *site) {
    workingSiteLineEdit->setText(site->getSiteName());
}
