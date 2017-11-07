#include "p_pb_playlistofday.h"

/**
     * Generic method to override for updating the presention.
     **/

P_PBPlayListOfDay::P_PBPlayListOfDay(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->setStyleSheet("background-color: #222");
    this->layout = new QGridLayout();
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    layout->setMargin(0);
    layout->setSpacing(5);
    this->zone->setLayout(layout);
    currentDate = QDateTime::currentDateTime();
    QDate date = currentDate.date();
    setSelectedDate(currentDate);
    displayPlaylistButton();
}
//datamap

void P_PBPlayListOfDay::updateStateOfHours(QList<int> mapStateHours){
    for (int index = 0; index < mapStateHours.size(); ++index) {
        if(index == this->selectedHour) continue;
        QString day1 = currentDate.date().toString("dd:MM:yyyy");
        QString day2 = this->selectedDate.date().toString("dd:MM:yyyy");
        if(day1 == day2){
            if (index > currentDate.time().hour()) {
                continue;
            }
        }
        int stateOfHour = mapStateHours.at(index);
        QPushButton *button = buttons.at(index);
        if(stateOfHour == 1){
            //neu co du du lieu
            button->setStyleSheet("color: #428bca;");
        }else if(stateOfHour == 2){
            //neu co phan co co phan khong
            button->setStyleSheet("color: #f0ad4e;");
        } else if(stateOfHour == 0  || stateOfHour == -1){
            //khong co du lieu
            button->setStyleSheet("color: #c9302c;");
        }
        else if(stateOfHour == -2){
            //reset Datamap
            displayPlaylistButton();
            displaySelectedPlayListITem();
            break;
        }
    }

}

void P_PBPlayListOfDay::updateTimeCurrent() {
    clearPlaylistButton();
    displayPlaylistButton();
}

void P_PBPlayListOfDay::displayPlaylistButton() {
    currentDate = QDateTime::currentDateTime();

    buttons.clear();
    for (int index = 0; index < 24; ++index) {
        QPushButton *button = new QPushButton(this->zone);
        button->setStyleSheet("background-color: black;");
        button->setFixedSize(60, 25);
        button->setText(QString::number(100 + index).right(2) + ":00");

        buttons.append(button);
        int r = index / col;
        int c = index - r * col;
        this->layout->addWidget(button, r, c);
        connect(button, &QPushButton::clicked, this,
                &P_PBPlayListOfDay::onPlaylistItemSelected);

        QString day1 = currentDate.date().toString("dd:MM:yyyy");
        QString day2 = this->selectedDate.date().toString("dd:MM:yyyy");
        if (day1 == day2) {
            if (index > currentDate.time().hour()) {
                button->setStyleSheet("background-color: gray;");
                button->setDisabled(true);
            } else if (index == this->selectedHour) {
                button->setStyleSheet("background-color: red;");
            }
        }
    }
}

void P_PBPlayListOfDay::setSelectedDate(QDateTime newDate) {
    currentDate = QDateTime::currentDateTime();
    clearPlaylistButton();
    this->selectedDate = newDate;
    displayPlaylistButton();
    displaySelectedPlayListITem();
    QString day1 = currentDate.date().toString("dd:MM:yyyy");
    QString day2 = this->selectedDate.date().toString("dd:MM:yyyy");

    if (day1 == day2) {
        // nếu ngày khi chọn lại ngày hiện tại nếu giờ được chọn vượt quá thời gian
        // hiện tại thì chọn đúng giờ hiện tại
        if (this->selectedHour > currentDate.time().hour()) {
            this->selectedHour = currentDate.time().hour();
            onPlaylistItemSelected();
        }
    }
    if (day1 == day2) {
        for (int index = 0; index < buttons.size(); ++index) {
            QPushButton *button = buttons.at(index);

            if (index > currentDate.time().hour()) {
                button->setStyleSheet("background-color: gray;");
                button->setDisabled(true);
            } else if (index == this->selectedHour) {
                button->setStyleSheet("background-color: red;");
            }
        }
    }
    seekToNewPosition();
}

void P_PBPlayListOfDay::displaySelectedPlayListITem() {
    currentDate = QDateTime::currentDateTime();

    QString day1 = currentDate.date().toString("dd:MM:yyyy");
    QString day2 = this->selectedDate.date().toString("dd:MM:yyyy");
    if (day1 == day2) {
        for (int index = 0; index < buttons.size(); ++index) {
            QPushButton *button = buttons.at(index);
            if (index > currentDate.time().hour()) {
                button->setStyleSheet("background-color: gray;");
                button->setDisabled(true);
            } else if (index == this->selectedHour) {
                button->setStyleSheet("background-color: red;");
            } else {
                button->setStyleSheet("background-color: black;");
            }
        }
    } else {
        for (int index = 0; index < buttons.size(); ++index) {
            QPushButton *button = buttons.at(index);
            if (this->selectedHour == index) {
                button->setStyleSheet("background-color: red;");
            } else {
                button->setStyleSheet("background-color: black;");
            }
        }
    }
}

void P_PBPlayListOfDay::clearPlaylistButton() {
    while (buttons.size() > 0) {
        QPushButton *button = buttons.last();
        button->hide();
        layout->removeWidget(button);
        buttons.removeLast();
        button->deleteLater();
    }
}

void P_PBPlayListOfDay::onPlaylistItemSelected() {
    currentDate = QDateTime::currentDateTime();
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != Q_NULLPTR) {
        this->selectedHour = buttons.indexOf(button);
        if (this->selectedHour >= 0) {
            displaySelectedPlayListITem();
            long beginTimeStamp = this->selectedDate.toTime_t() + selectedHour * 3600;
            qDebug() << "timestamps" << beginTimeStamp;
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(beginTimeStamp);
            this->control()->newUserAction(Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION,
                                           dataStruct);
        }
    }
}

void P_PBPlayListOfDay::update(int message, QVariant *attachment) {
    Q_UNUSED(attachment) Q_UNUSED(message)
}

QObject *P_PBPlayListOfDay::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

void P_PBPlayListOfDay::seekToNewPosition() {
    long newPosition = this->selectedDate.toTime_t() + this->selectedHour * 3600;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(newPosition);
    this->control()->newUserAction(Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION,
                                   dataStruct);

    QVariant *dataStruct1 = new QVariant();
    dataStruct1->setValue<int>(this->selectedHour);
    this->control()->newUserAction(Message.APP_PLAY_BACK_SELECTED_TIME_SLOT,
                                   dataStruct1);
}

void P_PBPlayListOfDay::selectDefaulTimeSlot() {
    this->selectedHour = QDateTime::currentDateTime().time().hour();
    this->seekToNewPosition();
}
