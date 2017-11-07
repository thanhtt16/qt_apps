#include "p_setting_videowall.h"

/**
     * Generic method to override for updating the presention.
     **/

P_SettingVideoWall::P_SettingVideoWall(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->setStyleSheet("color:#4c4b52");
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    this->zone->setLayout(mainLayout);

    QWidget *firstWidget = new QWidget(this->zone);
    QHBoxLayout *firstLayout = new QHBoxLayout();
    firstWidget->setFixedHeight(80);

    firstWidget->setLayout(firstLayout);
    mainLayout->addWidget(firstWidget);

    QWidget *leftWidget = new QWidget(firstWidget);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftWidget->setLayout(leftLayout);
    firstLayout->addWidget(leftWidget);

    QLabel *labeltransition = new QLabel(leftWidget);
    labeltransition->setText("Bắt đầu quá trình chuyển đổi");
    leftLayout->addWidget(labeltransition);
    comboBoxTransition = new QComboBox(leftWidget);

    comboBoxTransition->addItem(tr("Khi nhấp chuột"), QVariant(0));
    comboBoxTransition->addItem(tr("Tự động"), QVariant(1));
    QSettings settings;
    settings.beginGroup(QString::number(control()->getAppContext()->getWorkingUser()->getUserId()));

    int indexSessionSelected = 0;
    int pageTransitionMode = settings.value("page_transition_start_mode").toInt();

    if(pageTransitionMode >0 ){
        if(pageTransitionMode  == 202){
            indexSessionSelected = 1;
        }
    }

    comboBoxTransition->setCurrentIndex(indexSessionSelected);

    connect(comboBoxTransition, SIGNAL(currentIndexChanged(int)), this,
            SLOT(selectedTransition(int)));



    leftLayout->addWidget(comboBoxTransition);

    QWidget *rightWidget = new QWidget(firstWidget);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightWidget->setLayout(rightLayout);
    firstLayout->addWidget(rightWidget);

    QLabel *labelDelay = new QLabel(rightWidget);
    labelDelay->setText("Thời gian trì hoãn");
    rightLayout->addWidget(labelDelay);

    comboBoxDelay = new QComboBox(rightWidget);
    int indexTimeDelay = 0;
    int timeDelay = settings.value("page_transition_delay").toInt();
    if(timeDelay >0){
        indexTimeDelay = ((timeDelay/300) - 1);
    }
    comboBoxDelay->addItem(tr("5 Phút"),  QVariant(5));
    comboBoxDelay->addItem(tr("10 Phút"), QVariant(10));
    comboBoxDelay->addItem(tr("15 Phút"), QVariant(15));
    comboBoxDelay->addItem(tr("20 Phút"), QVariant(20));
    comboBoxDelay->addItem(tr("25 Phút"), QVariant(25));
    comboBoxDelay->addItem(tr("30 Phút"), QVariant(30));
    comboBoxDelay->addItem(tr("35 Phút"), QVariant(35));
    comboBoxDelay->addItem(tr("40 Phút"), QVariant(40));

    comboBoxDelay->setCurrentIndex(indexTimeDelay);

    connect(comboBoxDelay, SIGNAL(currentIndexChanged(int)), this,
            SLOT(selectedComboBoxDelay(int)));


    rightLayout->addWidget(comboBoxDelay);
}
void P_SettingVideoWall::selectedTransition(int index) {
    int valueSelected = comboBoxTransition->itemData(index).toInt();
    if (valueSelected == 0) {
        control()->newUserAction(Message.PAGE_TRANSITION_START_ON_CLICK, Q_NULLPTR);
    } else if (valueSelected == 1) {
        control()->newUserAction(Message.PAGE_TRANSITION_START_AUTOMATICALLY,
                                 Q_NULLPTR);
    }
}

void P_SettingVideoWall::selectedComboBoxDelay(int index) {
    int timeDelaySelected = comboBoxDelay->itemData(index).toInt();
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<int>(timeDelaySelected*60);
    control()->newUserAction(Message.PAGE_TRANSITION_DELAY, dataStruct);
}

void P_SettingVideoWall::update() {}

QObject *P_SettingVideoWall::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}
