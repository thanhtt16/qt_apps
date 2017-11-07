#include "p_setting_playback.h"

/**
     * Generic method to override for updating the presention.
     **/

P_SettingPlayBack::P_SettingPlayBack(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->setStyleSheet("color:#4c4b52");
    QPushButton *browseButton = new QPushButton(tr("&Browse..."), this->zone);

    connect(browseButton, &QAbstractButton::clicked, this,
            &P_SettingPlayBack::browse);

    QSettings settings;
//    settings.beginGroup(QString::number(control()->getParent()->getAppContext()->getWorkingUser()->getUserId()));
    QString pathDefault  = settings.value("path_save_media").toString();

    if (pathDefault.isEmpty()) {
        QString directory = QDir::homePath() + "/" + "Pictures";
        directoryComboBox = createComboBox(QDir::toNativeSeparators(directory));
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<QString>(directory);
        control()->newUserAction(Message.APP_PLAY_BACK_SET_PATH_SAVE_MEDIA,
                                 dataStruct);

    } else {
        directoryComboBox = createComboBox(QDir::toNativeSeparators(pathDefault));
    }

    connect(directoryComboBox->lineEdit(), &QLineEdit::returnPressed, this,
            &P_SettingPlayBack::animateFindClick);

    QGridLayout *mainLayout = new QGridLayout(this->zone);
    mainLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->setSpacing(15);
    this->zone->setLayout(mainLayout);

    mainLayout->addWidget(new QLabel(tr("Thư mục mặc định lưu hình:")), 0, 0);
    mainLayout->addWidget(directoryComboBox, 0, 1);
    mainLayout->addWidget(browseButton, 0, 2);
}

QComboBox *P_SettingPlayBack::createComboBox(const QString &text) {
    QComboBox *comboBox = new QComboBox;
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

void P_SettingPlayBack::animateFindClick() { findButton->animateClick(); }
void P_SettingPlayBack::browse() {
    QString path = QDir::homePath() + "/" + "Pictures";
    QString findFile = "Find files";
    QString directory = QDir::toNativeSeparators(
                QFileDialog::getExistingDirectory(this->zone, findFile, path));

    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1)
            directoryComboBox->addItem(directory);
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
        qDebug() << "path current " << directory;
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<QString>(directory);
        control()->newUserAction(Message.APP_PLAY_BACK_SET_PATH_SAVE_MEDIA,
                                 dataStruct);
    }
}

QObject *P_SettingPlayBack::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}
