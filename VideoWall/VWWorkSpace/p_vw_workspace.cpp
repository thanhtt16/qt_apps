#include "p_vw_workspace.h"
#include "Authentication/appcontext.h"
#include "Common/resources.h"
/**
     * Generic method to override for updating the presention.
     **/

P_VWWorkSpace::P_VWWorkSpace(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->show();
    this->zone->installEventFilter(this);
    this->zone->setMouseTracking(true);

    // create panels for grid
    for (int index = 0; index < Message.APP_VIDEO_WALL_MAX_NUMBER_OF_PLAYERS;
         ++index) {
        QWidget *playerZone = new QWidget(this->zone);
        playerZoneList.append(playerZone);
    }

    this->selectedLayoutLast.numberOfCameras = -1;
    this->selectedLayout.numberOfCameras = -1;

    // init layout
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(4);
    gridLayout->setMargin(0);
    this->zone->setLayout(gridLayout);

    //cdn Type Default
    networkTypeWorking = control()->appContext->getNetworkType();
}

void P_VWWorkSpace::updateNetworkTypeSelected(CamItemType networkTypeSelected){
    networkTypeWorking = networkTypeSelected;
    refreshNewLayoutPage();// refresh lại khi chọn luồng chơi mới
}


void P_VWWorkSpace::stopPlaying() {
    qDebug() << Q_FUNC_INFO;

    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(camIndex));
        cPlayer->newAction(Message.PLAYER_SOURCE_CLEAR, Q_NULLPTR);
    }
}

void P_VWWorkSpace::updateDefaulLayout(LayoutStruct defaultLayout) {
    if (!defaultLayout.label.isEmpty()) {
        this->selectedLayout = defaultLayout;
        if(this->control()->appContext->getWorkingApp().appName == appName){
            refresh(this->selectedLayout);
        }
    }
}

void P_VWWorkSpace::refreshAppShow() {
    qDebug() << Q_FUNC_INFO;
    if (!this->selectedLayout.label.isEmpty()) {
        refresh(this->selectedLayout);
    }
}

void P_VWWorkSpace::refresh(LayoutStruct layoutNew) {
    int idWorkingSite = control()->appContext->getWorkingSite()->getSiteId();
    QString tokenUserWorking =
            control()->appContext->getWorkingUser()->getToken();
    int pageNumber = layoutNew.selectedPage;
    int layoutNumber = layoutNew.numberOfCameras;

    //send message load list cameras of site with page and layout
    if(!firstRefresh){
        if(!layoutNew.label.isEmpty()){
            this->selectedLayout = layoutNew;
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue<LayoutStruct>(layoutNew);
            control()->newUserAction(Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE, dataStruct);
        }
    }else{
        firstRefresh = false;
        refreshNewLayoutPage();
    }


    //api 003

    //    if(!layoutNew.label.isEmpty()){
    //        this->selectedLayout = layoutNew;
    //        QVariant *dataStruct = new QVariant();
    //        dataStruct->setValue<LayoutStruct>(layoutNew);
    //        control()->newUserAction(Message.APP_VIDEO_WALL_GET_CAMERAS_OF_SITE_WITH_SITE_ID, dataStruct);
    //    }

    // get all cam of site
    /*  CamSite::getCamerasBy(
      idWorkingSite, tokenUserWorking,
      [this](CamSite *camSites) {
        control()->appContext->setListCamItemsOfSite(camSites->getCamItems());
      },
      [this](QString message) { qDebug() << Q_FUNC_INFO << message; })*/;
}

void P_VWWorkSpace::refreshNewLayoutPage(){
    // get list camItem with layoutpage
    pageCameras = this->control()->appContext->getSiteCameras()->getCamItems(); //get all camares of layoutpage current

    for (int index = 0; index < playerZoneList.size(); ++index) {
        QWidget *playerZone = playerZoneList.at(index);
        playerZone->hide();
        playerZone->lower();
        gridLayout->removeWidget(playerZone);
    }

    if (selectedIndexCurrent != -1) {
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName,
                    QString::number(selectedIndexCurrent));
        cPlayer->newAction(Message.PLAYER_EXIT_POP_OUT_MODE, Q_NULLPTR);
    }


    for (int row = 0; row < this->selectedLayout.rows; ++row) {
        for (int col = 0; col < this->selectedLayout.cols; ++col) {
            int zoneIndex = row * this->selectedLayout.cols + col;
            if (zoneIndex < playerZoneList.size()) {
                QWidget *playerZone = playerZoneList.at(zoneIndex);
                this->gridLayout->addWidget(playerZone, row, col);
                playerZone->show();
            }
        }
    }


    if (this->selectedLayout.cols > 0 && this->selectedLayout.rows > 0) {
        videoSize = QSize(this->zone->width() / this->selectedLayout.cols,
                          this->zone->height() / this->selectedLayout.rows);
    }

    numberOfPlayingPlayer = 0;

    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        CamItem *camItem = pageCameras.at(camIndex);
        if (camItem != Q_NULLPTR) {
            // count actual player used in the page
            numberOfPlayingPlayer++;

            camItem->setIsMain(pageCameras.size() <= 0 ? true : false);
            camItem->setNetworkType(networkTypeWorking);
            camItem->setOrder(camIndex);

            C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                        control(), Q_NULLPTR, appName, QString::number(camIndex));
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(camItem);

            cPlayer->newAction(Message.PLAYER_NEW_LIVE_SOURCE_SET,
                               dataStruct);
        }
    }

    if (pageCameras.size() < this->selectedLayoutLast.numberOfCameras) {
        for (int index = pageCameras.size();
             index < this->selectedLayoutLast.numberOfCameras; ++index) {
            C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                        control(), Q_NULLPTR, appName, QString::number(index));
            cPlayer->newAction(Message.PLAYER_SOURCE_CLEAR, Q_NULLPTR);
        }
    }

    this->selectedLayoutLast = this->selectedLayout;
}

void P_VWWorkSpace::networkChangeStatus(int message, QVariant *attachement) {
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(camIndex));
        cPlayer->newAction(message, attachement);
    }
}

void P_VWWorkSpace::show(QWidget *zone) { Q_UNUSED(zone) }

void P_VWWorkSpace::update() {}

QWidget *P_VWWorkSpace::getZone(int zoneId) {
    switch (zoneId) {
    default:
        return playerZoneList.at(zoneId);
    }
}

void P_VWWorkSpace::showFullCellAt(int index) {
    if (index < this->numberOfPlayingPlayer) {
        this->selectedIndex = index;
        this->selectedIndexCurrent = index;
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));

        QWidget *playerZone = cPlayer->presentation()->getZone();
        gridLayout->removeWidget(playerZone);
        playerZone->raise();
        QPropertyAnimation *geometryAnimation =
                new QPropertyAnimation(playerZone, "geometry");
        geometryAnimation->setDuration(animationDuration);
        QRect startValue = QRect(playerZone->x(), playerZone->y(),
                                 playerZone->width(), playerZone->height());
        QRect endValue = QRect(0, 0, this->zone->width(), this->zone->height());
        geometryAnimation->setStartValue(startValue);
        geometryAnimation->setEndValue(endValue);
        geometryAnimation->setEasingCurve(QEasingCurve::Linear);
        geometryAnimation->start(QPropertyAnimation::DeleteWhenStopped);
        connect(geometryAnimation, &QPropertyAnimation::finished, this,
                [this, cPlayer] {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(cPlayer);
            this->control()->newAction(Message.PLAYER_END_SHOW_FULLSCREEN,
                                       dataStruct);
        });
    }
}

void P_VWWorkSpace::hideFullCellAt(int index) {
    if (index < numberOfPlayingPlayer) {
        selectedIndex = index;
        selectedIndexCurrent = index;

        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));

        int row = selectedIndex / this->selectedLayout.rows;
        int col = selectedIndex - row * this->selectedLayout.rows;
        QWidget *playerZone = cPlayer->presentation()->getZone();
        playerZone->raise();

        QPropertyAnimation *geometryAnimation =
                new QPropertyAnimation(playerZone, "geometry");
        geometryAnimation->setDuration(animationDuration);
        videoSize = QSize(this->zone->width() / this->selectedLayout.cols,
                          this->zone->height() / this->selectedLayout.rows);
        QRect startValue = QRect(playerZone->x(), playerZone->y(),
                                 playerZone->width(), playerZone->height());
        QRect endValue = QRect(col * videoSize.width(), row * videoSize.height(),
                               videoSize.width(), videoSize.height());

        geometryAnimation->setStartValue(startValue);
        geometryAnimation->setEndValue(endValue);
        geometryAnimation->setEasingCurve(QEasingCurve::Linear);
        geometryAnimation->start(QPropertyAnimation::DeleteWhenStopped);
        connect(geometryAnimation, &QPropertyAnimation::finished, this,
                [this, cPlayer, playerZone] {
            int row = selectedIndex / this->selectedLayout.rows;
            int col = selectedIndex - row * this->selectedLayout.rows;
            this->gridLayout->addWidget(playerZone, row, col);
            this->selectedIndex = -1;
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(cPlayer);
            this->control()->newAction(Message.PLAYER_END_HIDE_FULLSCREEN,
                                       dataStruct);
        });
    }

}


void P_VWWorkSpace::resizePlayerWhenEnterFullScreen(){
    //neu van dang full screen  thi resize player
    if(selectedIndex != - 1){
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        cPlayer->newAction(Message.ENTER_FULLSCREEN_MODE, Q_NULLPTR);
    }

}
void P_VWWorkSpace::resizePlayerWhenExitFullScreen(){
    //neu van dang full screen  thi resize player
    if(selectedIndex != - 1){
        C_Cam9RTCPlayer *cPlayer = Cam9PlayerPool::instance().getCam9RTCPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        cPlayer->newAction(Message.EXIT_FULLSCREEN_MODE, Q_NULLPTR);
    }

}
bool P_VWWorkSpace::eventFilter(QObject *watched, QEvent *event) {
    Q_UNUSED(watched);
    switch (event->type()) {
    default:
        break;
    }
    return true;
}
