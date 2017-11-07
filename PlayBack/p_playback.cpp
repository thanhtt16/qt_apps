#include "PlayBack/p_playback.h"
#include <QWidget>
/**
     * Generic method to override for updating the presention.
     **/

P_PlayBack::P_PlayBack(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    qDebug() << "START P_PLAYBACK";
    QSize screenSize = Resources::instance().getScreenSize();
    workSpace = new QWidget(zone);
    //  workSpace->setFixedWidth(screenSize.width() - appSize.rightWidth);
    //  workSpace->setFixedHeight(screenSize.height() - appSize.topHeight);
    rightBar = new QWidget(zone);
    rightBar->setFixedWidth(appSize.rightWidth);

    this->layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignLeft);
    zone->setLayout(this->layout);
    layout->addWidget(workSpace);
    layout->addWidget(rightBar);
    qDebug() << "P_PlayBack Success";

}

void P_PlayBack::show(QWidget *zone) { Q_UNUSED(zone) }

void P_PlayBack::update() {}

QWidget *P_PlayBack::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return workSpace;
    case 2:
        return rightBar;
    default:
        return Q_NULLPTR;
    }
}

void P_PlayBack::showRightBar() {}

void P_PlayBack::siteChanged() {}

void P_PlayBack::enterFullscreenMode() {
    rightBar->setFixedWidth(0);
    workSpace->resize(Resources::instance().getScreenSize());
}

void P_PlayBack::exitFullscreenMode() {
    rightBar->setFixedWidth(appSize.rightWidth);
}
