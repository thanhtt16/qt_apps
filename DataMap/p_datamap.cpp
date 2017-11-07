#include "p_datamap.h"


P_DataMap::P_DataMap(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    this->zone = zone;
    this->zone->installEventFilter(this);
    this->zone->setMouseTracking(true);

    this->seekBarWidget = new SeekBarWidget(this->zone);
    this->layout = new QVBoxLayout();
    this->layout->addWidget(this->seekBarWidget);
    this->zone->setLayout(this->layout);

    //  startTimeLabel = new QLabel(this->zone);
    //  endTimeLabel = new QLabel(this->zone);

    //  startTimeLabel->setStyleSheet("background-color: black; color: white");
    //  endTimeLabel->setStyleSheet("background-color: black; color: white");

    //  startTimeLabel->setFixedSize(80, 20);
    //  endTimeLabel->setFixedSize(80, 20);

    //  startTimeLabel->move(0, 0);
    //  endTimeLabel->move(0, 0);

    //  startTimeLabel->setText("00:00:00");
    //  endTimeLabel->setText("00:00:00");

    //  startTimeLabel->show();
    //  endTimeLabel->show();
}

void P_DataMap::update() {}

QObject *P_DataMap::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

//void P_DataMap::drawDataMapArray(int startTimestamp,
//                                 int timeDuration){

//    this->seekBarWidget->drawDataMapFrom(startTimestamp,timeDuration);
//}

bool P_DataMap::getIsMain() const { return isMain; }

void P_DataMap::setIsMain(bool value) { isMain = value; }
