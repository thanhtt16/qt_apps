#include "p_pb_workspace.h"
#include "Authentication/appcontext.h"
#include "Common/resources.h"

/**
     * Generic method to override for updating the presention.
     **/

TimeRange P_PBWorkSpace::getTimeRangeCurrent() const {
    return timeRangeCurrent;
}

void P_PBWorkSpace::setTimeRangeCurrent(const TimeRange &value) {
    timeRangeCurrent = value;
    //update timestamp to player used replay
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(camIndex));
        if(cPlayer){
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue<TimeRange>(timeRangeCurrent);
            cPlayer->newAction(Message.APP_PLAY_BACK_UPDATE_TIMESTAMP_FOR_PLAYER, dataStruct);
        }
    }
}

P_PBWorkSpace::P_PBWorkSpace(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
    this->zone = zone;
    this->zone->show();
    this->zone->installEventFilter(this);
    this->zone->setMouseTracking(true);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    this->zone->setLayout(layout);
    gridPlayback = new QWidget(this->zone);
    seekBar = new QWidget(this->zone);
    seekBar->setStyleSheet("background-color: #ecf0f1;");
    seekBar->setFixedHeight(50);

    recorder = new QWidget(this->zone);
    recorder->setFixedSize(600, 490);
    recorder->setStyleSheet("background:#ecf0f1");
    recorder->hide();
    layout->addWidget(gridPlayback);
    layout->addWidget(seekBar);
    // create panels for grid
    for (int index = 0; index < Message.APP_PLAY_BACK_MAX_NUMBER_OF_PLAYERS;
         ++index) {
        QWidget *playerZone = new QWidget(gridPlayback);
        playerZoneList.append(playerZone);
    }
    this->selectedLayoutLast.numberOfCameras = -1;
    this->selectedLayout.numberOfCameras = -1;
    // init layout

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(4);
    gridLayout->setMargin(0);
    gridPlayback->setLayout(gridLayout);
    mVersionApi = AppProfile::getAppProfile()->getAppConfig()->getApiVodVersion();
    this->mVideoVOD = new VideoVOD(Q_NULLPTR);
    this->dataMapOfCamera = new DataMapOfCamera();
    networkTypeWorking = control()->appContext->getNetworkType();
}


void P_PBWorkSpace::updateNetworkTypeSelected(CamItemType networkTypeSelected){
    networkTypeWorking = networkTypeSelected;
}

//Load toàn bộ datamap của camera với số lượng ngày historyday
void P_PBWorkSpace::loadDataMapOfCameraAndUpdateGui(bool isMain, int historyDay, int cameraId){
    function<void(QString)> onFailure = [](QString message) {
        qDebug() << "Load DataMap Failure";
    };

    function<void(DataMapOfCamera *)> onSuccess =
            [this,historyDay,isMain](DataMapOfCamera *dataMapOfCamera) {
        this->dataMapOfCamera = dataMapOfCamera;

        QList<QDate> listDaysMap = this->dataMapOfCamera->getListDaysMap(isMain);

        //        long unixTimeDateCurrent = QDateTime::currentDateTime().toTime_t();
        //        int timestampofoneday = 24 * 60 * 60;
        //        QList<QDate>  listHistoryDates;
        //        for (int index = 0; index < historyDay; ++index) {
        //            QDateTime datetime;
        //            datetime.setTime_t(unixTimeDateCurrent);
        //            listHistoryDates.append(datetime.date());
        //            unixTimeDateCurrent -= timestampofoneday;
        //        }

        //update state of day in calendar
        this->updateStateOfDaysInMonth(isMain, listDaysMap);

        QList<QString> hours;
        for (int index = 0; index < 24; ++index) {
            hours.append(QString::number(index));
        }
        this->updateStateHours(isMain, hours, dateSelected);

        this->hourSelected = QDateTime::fromSecsSinceEpoch(this->beginOfTimeSlot).time().hour();
        updateStateSeekbar(isMain, this->dateSelected,  this->hourSelected);

        qDebug() << "========================================== DATAMAP-START ============================================";
        //      qDebug() << "THAO GET STATE" <<  dataMapOfCamera->getSateDataOfDay(false,"2017/9/19");
        //      qDebug() << "THAO GET STATE _ HOUR" << dataMapOfCamera->getStateHourOfDay(false,"2017/9/19",21);

        //      qDebug() <<"DATA_DAY_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.daysMap.size();
        //      qDebug() <<"DATA_DETAIl_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.detailMap.size();
        //      qDebug() <<"DATA_HOURS_MAP_PARSER" <<dataMapOfCamera->sdDataMapOfCamera.hoursMap.size();

        //        this->getBookingDate().toString("yyyy.MM.dd")

        //                int unixTime = 1234567890;
        //                QDateTime timestamp;
        //                timestamp.setTime_t(unixTime);
        //                qDebug() << timestamp.toString(Qt::SystemLocaleShortDate);



        //        QDate date = QDate::fromString("2017/9/26","yyyy/M/dd");
        //        qDebug() << "DATE CONVERT " << date;

        //        QList<int> listHourMap  = dataMapOfCamera->createMapHourForSeekBar(isMain,date, 3);


    };

    int hour = QDateTime::currentDateTime().time().hour();
    int munite = QDateTime::currentDateTime().time().minute();
    int second = QDateTime::currentDateTime().time().second();
    qDebug() << "HISTORYDAY SAVED  " << historyDay <<"timestamp" << QDateTime::currentDateTime().toTime_t()  <<  (86000 - (hour*3600 + munite*60 + second)) << "hour" << hour << munite <<second;
    long unixtimeToDay = QDateTime::currentDateTime().toTime_t() + (86400 - (hour*3600 + munite*60 + second));
    long unixtimeLateHistoryday = unixtimeToDay - historyDay * 24 * 60 * 60;
    qDebug() << "UNIXTIME" <<"START" << unixtimeLateHistoryday << "END" << unixtimeToDay;
    DataMapOfCamera::loadDataMapOfCamera(unixtimeLateHistoryday,unixtimeToDay,cameraId, onSuccess,
                                         onFailure);
}

void P_PBWorkSpace::updateStateSeekbar(bool isMain,QDate dateSelected, int hourSelectedCurrent){
    QList<int> listMapHourOfCamera =  this->dataMapOfCamera->createMapHourForSeekBar(isMain,dateSelected, hourSelectedCurrent);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<int>>(listMapHourOfCamera);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_SEEK_BAR, dataStruct);
}

void P_PBWorkSpace::resetStateSeekbar(){

    QList<int> listMapHourOfCamera;
    for (int index = 0; index < 3600; ++index) {
        listMapHourOfCamera.append(-1);
    }

    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<int>>(listMapHourOfCamera);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_SEEK_BAR, dataStruct);
}




void P_PBWorkSpace::updateStateHours(bool isMain, QList<QString> hours,QDate dateSelected){
    QList<int> listMapStateHours = this->dataMapOfCamera->getSateDataOfHours(isMain,hours, dateSelected);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<int>>(listMapStateHours);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS, dataStruct);
}

void P_PBWorkSpace::resetStateHours(){
    QList<int> listMapStateHours;
    for (int index = 0; index < 24; ++index) {
        listMapStateHours.append(-2);
    }

    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<int>>(listMapStateHours);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_HOURS, dataStruct);
}

void P_PBWorkSpace::updateDateSelectedCurrent(QDate dateSelectedCurrent){
    this->dateSelected = dateSelectedCurrent;
}

void P_PBWorkSpace::updateStateOfDaysInMonth(bool isMain, QList<QDate> listDates){

    QList<QHash<QDate, int>> resultStates;
    resultStates = this->dataMapOfCamera->getSateDataOfDates(isMain, listDates);

    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<QHash<QDate, int>>>(resultStates);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR, dataStruct);
}

void P_PBWorkSpace::resetStateOfCalendar(){
    QList<QHash<QDate, int>> resultStates;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<QList<QHash<QDate, int>>>(resultStates);
    this->control()->newAction(Message.APP_PLAY_BACK_UPDATE_DATAMAP_CALENDAR, dataStruct);
}

void P_PBWorkSpace::refresh(LayoutStruct layout) {
    this->isShowFullScreen = 0;
    if (!layout.label.isEmpty()) {
        this->selectedLayout = layout;
        this->refreshAppShow();
    }
}

void P_PBWorkSpace::updateDefaultLayout(LayoutStruct defaultLayout) {
    if (!defaultLayout.label.isEmpty()) {
        this->selectedLayout = defaultLayout;
        //if is first not refresh
        if(this->control()->appContext->getWorkingApp().appName == appName){
            qDebug() << "CHUYEN SITE REFRESH";
            this->refreshNewLayoutPage();
        }
    }
}

void P_PBWorkSpace::seekToNewTimeSlot(long beginOfTimeSlot) {
    this->beginOfTimeSlot = beginOfTimeSlot;
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(camIndex));

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(beginOfTimeSlot);
        cPlayer->newAction(Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION, dataStruct);
    }
}

void P_PBWorkSpace::seekToNewPosition(long newPosition) {
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(camIndex));

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(newPosition);
        cPlayer->newAction(Message.APP_PLAY_BACK_SEEK_TO_NEW_POSITION, dataStruct);
    }
}

void P_PBWorkSpace::refreshAppShow() {
    if (!this->selectedLayout.label.isEmpty()) {
        int idWorkingSite = control()->appContext->getWorkingSite()->getSiteId();
        QString tokenUserWorking =
                control()->appContext->getWorkingUser()->getToken();
        int pageNumber = this->selectedLayout.selectedPage;
        int layoutNumber = this->selectedLayout.numberOfCameras;

        //api 004
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<LayoutStruct>(this->selectedLayout);
        control()->newUserAction(Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_LAYOUT_PAGE, dataStruct);

        //api OO3
        //        QVariant *dataStruct = new QVariant();
        //        dataStruct->setValue<LayoutStruct>(this->selectedLayout);
        //        control()->newUserAction(Message.APP_PLAY_BACK_GET_CAMERAS_OF_SITE_WITH_SITE_ID, dataStruct);



        // get all cam of site
        //    CamSite::getCamerasBy(
        //        idWorkingSite, tokenUserWorking,
        //        [this](CamSite *camSites) {
        //          control()->appContext->setListCamItemsOfSite(camSites->getCamItems());
        //        },
        //        [this](QString message) { qDebug() << Q_FUNC_INFO << message;
        //        });
    }
}

void P_PBWorkSpace::refreshNewLayoutPage(){
    this->isShowFullScreen = 0;
    // load camitem with layoutpage
    //    this->resetStateOfCalendar();
    //    this->resetStateHours();
    //    this->resetStateSeekbar();

    this->pageCameras = control()->appContext->getSiteCameras()->getCamItems();

    for (int index = 0; index < playerZoneList.size(); ++index) {
        QWidget *playerZone = playerZoneList.at(index);
        playerZone->hide();
        playerZone->lower();
        gridLayout->removeWidget(playerZone);
    }

    if (selectedIndexCurrent != -1) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
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

    if (selectedLayout.cols > 0 && selectedLayout.rows > 0) {
        videoSize =
                QSize(this->zone->width() / this->selectedLayout.cols,
                      this->zone->height() / this->selectedLayout.rows);
    }
    numberOfPlayingPlayer = 0;
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        CamItem *camItem = pageCameras.at(camIndex);
        if (camItem != Q_NULLPTR) {
            C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                        control(), Q_NULLPTR, appName, QString::number(camIndex));
            numberOfPlayingPlayer++;
        }
    }


    if (isSetPageDefault == false) {
        if (beginOfTimeSlot > 0) {
            isSetPageDefault = true;
            seekToNewPostion(beginOfTimeSlot);
        }
    } else {
        playListVideoVodWithTimeStamp(timeRangeCurrent);
    }

    if (pageCameras.size() < this->selectedLayoutLast.numberOfCameras) {
        for (int index = pageCameras.size();
             index < this->selectedLayoutLast.numberOfCameras; ++index) {
            C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                        control(), Q_NULLPTR, appName, QString::number(index));
            cPlayer->newAction(Message.PLAYER_SOURCE_CLEAR, Q_NULLPTR);
            //        cPlayer->newAction(Message.PLAYER_BEGIN_HIDE_FULLSCREEN,
            //        Q_NULLPTR);
        }
    }
    this->selectedLayoutLast =  this->selectedLayout;
}

void P_PBWorkSpace::show(QWidget *zone) { Q_UNUSED(zone) }

void P_PBWorkSpace::update() {}

QWidget *P_PBWorkSpace::getZone(int zoneId) {
    switch (zoneId) {
    case -1:
        return seekBar;
    case -2:
        return recorder;
    default:
        return playerZoneList.at(zoneId);
    }
}

void P_PBWorkSpace::stopPlaying() {
    for (int camIndex = 0; camIndex < pageCameras.size(); ++camIndex) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    control(), Q_NULLPTR, appName, QString::number(camIndex));
        cPlayer->newAction(Message.PLAYER_SOURCE_CLEAR, Q_NULLPTR);
    }
}

void P_PBWorkSpace::showFullCellAt(int index) {
    isShowFullScreen = 1;
    if (index < this->numberOfPlayingPlayer) {
        this->selectedIndexCurrent = index;
        this->selectedIndex = index;
        C_Cam9Player *player = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        QWidget *playerZone = player->presentation()->getZone();
        gridLayout->removeWidget(playerZone);
        playerZone->raise();

        QRect startValue = QRect(playerZone->x(), playerZone->y(),
                                 playerZone->width(), playerZone->height());
        QRect endValue =
                QRect(0, 0, this->gridPlayback->width(), this->gridPlayback->height());

        QPropertyAnimation *geometryAnimation =
                new QPropertyAnimation(playerZone, "geometry");
        geometryAnimation->setDuration(animationDuration);
        geometryAnimation->setStartValue(startValue);
        geometryAnimation->setEndValue(endValue);
        geometryAnimation->setEasingCurve(QEasingCurve::Linear);
        geometryAnimation->start(QPropertyAnimation::DeleteWhenStopped);
        connect(geometryAnimation, &QPropertyAnimation::finished, this,
                [this, player] {
            QVariant *dataStruct = new QVariant();
            dataStruct->setValue(player);
            this->control()->newAction(Message.PLAYER_END_SHOW_FULLSCREEN,
                                       dataStruct);
            CamItem *camItemSelected = pageCameras.at(selectedIndex);
            if(camItemSelected){
                //                int historyDayOfCamItem = 30; //default historyday
                //                int cameraId = camItemSelected->getCameraId();
                //                bool ok;
                //                int historyDay = camItemSelected->getHistoryDay().toInt(&ok);
                //                if(ok && historyDay!= 0) {
                //                    historyDayOfCamItem = historyDay;
                //                }

                //                loadDataMapOfCameraAndUpdateGui(false, historyDayOfCamItem, cameraId);
            }
        });
    }
}

void P_PBWorkSpace::hideFullCellAt(int index) {
    isShowFullScreen = 0;
    recorder->hide();
    if (index < this->numberOfPlayingPlayer) {
        isAnimating = true;
        selectedIndex = index;
        C_Cam9Player *player = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        int row = selectedIndex / this->selectedLayout.rows;
        int col = selectedIndex - row * this->selectedLayout.rows;
        QWidget *playerZone = player->presentation()->getZone();

        playerZone->raise();

        QRect startValue = QRect(playerZone->x(), playerZone->y(),
                                 playerZone->width(), playerZone->height());
        QRect endValue = QRect(col * videoSize.width(), row * videoSize.height(),
                               videoSize.width(), videoSize.height());
        QPropertyAnimation *geometryAnimation =
                new QPropertyAnimation(playerZone, "geometry");
        geometryAnimation->setDuration(animationDuration);
        if (startValue.width() != endValue.width() &&
                startValue.height() != endValue.height()) {
            geometryAnimation->setStartValue(startValue);
            geometryAnimation->setEndValue(endValue);
            geometryAnimation->setEasingCurve(QEasingCurve::Linear);
            geometryAnimation->start(QPropertyAnimation::DeleteWhenStopped);
            connect(geometryAnimation, &QPropertyAnimation::finished, this,
                    [this, player, playerZone] {
                int row = selectedIndex / this->selectedLayout.rows;
                int col = selectedIndex - row * this->selectedLayout.rows;
                this->gridLayout->addWidget(playerZone, row, col);
                this->selectedIndex = -1;
                QVariant *dataStruct = new QVariant();
                dataStruct->setValue(player);
                this->control()->newAction(Message.PLAYER_END_HIDE_FULLSCREEN,
                                           dataStruct);
                //                this->resetStateOfCalendar();
                //                this->resetStateHours();
                //                this->resetStateSeekbar();
            });
        }
    }
}

QString P_PBWorkSpace::getAppName() const { return appName; }

void P_PBWorkSpace::setAppName(const QString &value) { appName = value; }

void P_PBWorkSpace::fetchListVideoVODRequest(QList<QString> listCamera,
                                             TimeRange timeRange) {

    function<void(QString)> onFailure = [this](QString message) {
        Q_UNUSED(message)
        this->zone->setCursor(Qt::ArrowCursor);
    };

    function<void(QList<VideoVOD *>)> onSuccess =
            [this, timeRange](QList<VideoVOD *> listVideoVOD) {

        qDebug() << "fetchListVideoVODRequest" << listVideoVOD.size();
        this->listVideoVOD = listVideoVOD;

        for (int index = 0; index < listVideoVOD.size(); ++index) {
            VideoVOD *videoVod = this->listVideoVOD.at(index);
            CamItem *camItem = pageCameras.at(index);
            if (camItem != Q_NULLPTR) {
                camItem->setOrder(index);
                camItem->setTimeRange(timeRange);
                //                if (this->pageCameras.size() <= 4) {
                //                    videoVod->setModeCurrent("HD");
                //                }
                videoVod->setCamItem(camItem);
                camItem->setNetworkType(networkTypeWorking);

                C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                            control(), Q_NULLPTR, appName, QString::number(index));
                //          players.append(cPlayer);
                QVariant *dataStruct = new QVariant();
                dataStruct->setValue(videoVod);
                cPlayer->newAction(Message.PLAYER_NEW_VOD_SOURCE_SET, dataStruct);
                cPlayer->newAction(Message.PLAYER_PLAY, Q_NULLPTR);
            }
        }
        this->zone->setCursor(Qt::ArrowCursor);
    };

    this->zone->setCursor(Qt::BusyCursor);
    QString userToken = control()->appContext->getWorkingUser()->getToken();
    VideoVOD::fetchListVideoVOD(userToken, listCamera, timeRange, onSuccess,
                                onFailure);
}

void P_PBWorkSpace::fetchOneVideoVOD(QString cameraId,
                                     TimeRange timeRangeRequest) {


    qDebug() << Q_FUNC_INFO << "TIMERANGE" << timeRangeRequest.starttime
             << "LENGTH" << timeRangeRequest.length;

    CamItem *camItem = pageCameras.at(selectedIndexCurrent);
    camItem->setTimeRange(timeRangeRequest);
    camItem->setOrder(selectedIndexCurrent);

    if(camItem && camItem->isRtc()){
        //namrtc
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(selectedIndexCurrent));
        this->mVideoVOD->setSpeedScale(1.0);
        this->mVideoVOD->setCamItem(camItem);
        this->mVideoVOD->setIsShowFullSceen(isShowFullScreen);

        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(this->mVideoVOD);
        cPlayer->newAction(Message.PLAYER_NEW_VOD_SOURCE_ONE_VIDEO_SET,
                           dataStruct);
        cPlayer->newAction(Message.PLAYER_PLAY, Q_NULLPTR);

    }else{
        //namtv
        function<void(QString)> onFailure = [this](QString message) {
            this->zone->setCursor(Qt::ArrowCursor);
        };
        function<void(VideoVOD *)> onSuccess =
                [this, timeRangeRequest,camItem](VideoVOD *videoVOD) {
            if (videoVOD) {
                if (camItem->isTv()) {
                    C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                                this->control(), Q_NULLPTR, appName, QString::number(selectedIndexCurrent));
                    videoVOD->setSpeedScale(1.0);
                    camItem->setNetworkType(networkTypeWorking);

                    videoVOD->setCamItem(camItem);
                    videoVOD->setIsShowFullSceen(isShowFullScreen);
                    this->mVideoVOD = videoVOD;

                    QVariant *dataStruct = new QVariant();
                    dataStruct->setValue(videoVOD);
                    cPlayer->newAction(Message.PLAYER_NEW_VOD_SOURCE_ONE_VIDEO_SET,
                                       dataStruct);
                    cPlayer->newAction(Message.PLAYER_PLAY, Q_NULLPTR);

                }
            }
            this->zone->setCursor(Qt::ArrowCursor);
        };

        this->zone->setCursor(Qt::BusyCursor);
        QString userToken = control()->appContext->getWorkingUser()->getToken();
        VideoVOD::fetchOneVideoVOD(userToken, cameraId, timeRangeRequest, onSuccess,
                                   onFailure);
    }
}

//chon ngay moi
void P_PBWorkSpace::seekToNewPostion(long newPostion) {
    //nếu đang fullscreen và selected ngày mới cần vẽ lại datamap của giờ

    this->beginOfTimeSlot = newPostion;
    this->hourSelected = QDateTime::fromSecsSinceEpoch(this->beginOfTimeSlot).time().hour();

    if(isShowFullScreen == 1){
        QList<QString> hours;
        for (int index = 0; index < 24; ++index) {
            hours.append(QString::number(index));
        }
        this->updateStateHours(false, hours, dateSelected);
        this->updateStateSeekbar(false,dateSelected,this->hourSelected);
    }

    TimeRange timeRangeRequest;
    timeRangeRequest.starttime = newPostion;
    timeRangeRequest.length = 3600;

    QList<QString> listCamera;
    for (int index = 0; index < pageCameras.size(); ++index) {
        CamItem *camItem = pageCameras.at(index);
        if (camItem != Q_NULLPTR) {
            camItem->setOrder(index);
            camItem->setTimeRange(timeRangeRequest);
            qDebug() << Q_FUNC_INFO << "TIMERANGE" << timeRangeRequest.starttime
                     << "LENGTH" << timeRangeRequest.length;
            if (camItem->isRtc()) {
                C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                            this->control(), Q_NULLPTR, appName, QString::number(index));
                VideoVOD *videoVod = new VideoVOD(Q_NULLPTR);
                //                if (this->pageCameras.size() <= 4) {
                //                    videoVod->setModeCurrent("HD");
                //                }
                videoVod->setSpeedScale(1.0);
                camItem->setNetworkType(networkTypeWorking);
                videoVod->setCamItem(camItem);
                QVariant *dataStruct = new QVariant();
                dataStruct->setValue(videoVod);
                cPlayer->newAction(Message.PLAYER_NEW_VOD_SOURCE_SET, dataStruct);
                cPlayer->newAction(Message.PLAYER_PLAY, Q_NULLPTR);

            } else {
                //namtv
                listCamera.append(QString::number(camItem->getCameraId()));
            }
        }
    }

    if (listCamera.size() > 0) {
        fetchListVideoVODRequest(listCamera, timeRangeRequest);
    }
}

void P_PBWorkSpace::playListVideoVodWithTimeStamp(TimeRange timeRange) {
    QList<QString> listCamera;
    for (int index = 0; index < pageCameras.size(); ++index) {
        CamItem *camItem = pageCameras.at(index);
        if (camItem != Q_NULLPTR) {
            camItem->setOrder(index);
            camItem->setTimeRange(timeRange);
            qDebug() << Q_FUNC_INFO << "TIMERANGE" << timeRange.starttime << "LENGTH"
                     << timeRange.length;
            if (camItem->isRtc()) {
                C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                            this->control(), Q_NULLPTR, appName, QString::number(index));
                VideoVOD *videoVod = new VideoVOD(Q_NULLPTR);
                //        if (this->pageCameras.size() <= 4) {
                //          videoVod->setModeCurrent("HD");
                //        }
                videoVod->setSpeedScale(1.0);
                camItem->setNetworkType(networkTypeWorking);
                videoVod->setCamItem(camItem);
                QVariant *dataStruct = new QVariant();
                dataStruct->setValue(videoVod);
                cPlayer->newAction(Message.PLAYER_NEW_VOD_SOURCE_SET, dataStruct);
                cPlayer->newAction(Message.PLAYER_PLAY, Q_NULLPTR);
            } else {
                listCamera.append(QString::number(camItem->getCameraId()));
            }
        }
    }

    if (listCamera.size() > 0) {
        qDebug() << Q_FUNC_INFO << listCamera.size();
        fetchListVideoVODRequest(listCamera, timeRange);
    }



}

QString P_PBWorkSpace::liveToVod(QString live, TimeRange timeRange) {
    QString vod = "http://";
    QStringList lives = live.split("/");
    vod.append(lives.at(2));
    vod.append("/rec/hls/");
    vod.append(lives.last());
    vod.append("_");
    vod.append(QString::number(timeRange.starttime * 1000));
    vod.append("_");
    vod.append(QString::number(timeRange.length * 1000));
    vod.append(".m3u8");
    return vod;
}

void P_PBWorkSpace::playOneVideoVodWithTimeStamp(TimeRange timeRange) {
    QString cameraId;
    CamItem *camItemSelected = pageCameras.at(selectedIndexCurrent);
    if (camItemSelected) {
        cameraId = QString::number(camItemSelected->getCameraId());
    }
    // fetch du lieu
    fetchOneVideoVOD(cameraId, timeRange);
}


void P_PBWorkSpace::fetchOneVideoVODSwitchMode(long playerId) {
    selectedIndexCurrent = playerId;
    playOneVideoVodWithTimeStamp(timeRangeCurrent);
}

void P_PBWorkSpace::changeSpeedAllVideo(int message, QVariant *dataStruct) {
    for (int index = 0; index < this->numberOfPlayingPlayer; ++index) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(index));
        cPlayer->newAction(message, dataStruct);
    }
}
void P_PBWorkSpace::showRecordDetail() {
    recorder->show();
    recorder->move(this->zone->width() - 700,
                   this->zone->height() - recorder->height() - 90);
    recorder->raise();
}

void P_PBWorkSpace::hideRecordDetail() { recorder->hide(); }

void P_PBWorkSpace::recordVideoDefault() {
    qDebug() << Q_FUNC_INFO << "***********************RECORD DEFAULT***********************************";
    isRecordDefault = true;
    // neu chon record mac dinh
    if (checkRecord == 0) {
        startAnimationRecordNormalButton();
        mTimeStartRecordVOD = timeRangeCurrent.starttime;
        checkRecord = 1;
    } else {
        checkRecord = 0;
        stopAnimationRecordNormalButton();
        mTimeEndRecordVOD = timeRangeCurrent.starttime;
        secondRecord = mTimeEndRecordVOD - mTimeStartRecordVOD;

        // start download video record
        TimeRange timeRangeRecord;
        timeRangeRecord.starttime = mTimeStartRecordVOD;
        timeRangeRecord.length = secondRecord;

        QString cameraId;
        CamItem *camItemSelected = pageCameras.at(selectedIndex);
        if (camItemSelected) {
            if (camItemSelected->isTv()) {
                // record namtv
                cameraId = QString::number(camItemSelected->getCameraId());
                QString nameCamera = camItemSelected->getPostion();

                QString urlVideoVODRecord;
                // de lay link record theo luong thoi gian truyen vao
                fetchVideoURLVODRecord(cameraId, timeRangeRecord);

                if (mVersionApi == "2.0") {
                    urlVideoVODRecord =
                            this->mVideoVODRecord->getVideoVODProfile()->getSrc();
                } else if (mVersionApi == "3.0") {
                    urlVideoVODRecord = this->mVideoVODRecord->getSrcMain();
                }

                TypeTime tmp_TypeTime;
                tmp_TypeTime = GeneralDefine::instance().convertSecondsToTexts(
                            mTimeStartRecordVOD);
                QString dateFile =
                        tmp_TypeTime.day.append("_").append(tmp_TypeTime.time);
                QString fileNameChange = "Video_" + nameCamera + "_" + dateFile;
                fileNameChange.replace("/", "_");
                fileNameChange.replace(":", ".");
                InfomationRecordVOD *infoVideoRecord = new InfomationRecordVOD;

                infoVideoRecord->url = urlVideoVODRecord;
                infoVideoRecord->filename = fileNameChange;
                fetchPathRecordVODRecord(urlVideoVODRecord, "namtv", infoVideoRecord);

            } else {
                // record namrtc

                cameraId = QString::number(camItemSelected->getCameraId());
                QString nameCamera = camItemSelected->getPostion();

                C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                            this->control(), Q_NULLPTR, appName, QString::number(selectedIndex));

                CamItemType type;
                type.name = cPlayer->getModePlayerCurrent();

                // de lay link record theo luong thoi gian truyen vao
                CamStream *camStream = camItemSelected->getCamStream(type);
                if(camStream){
                    QString source = camStream->getSource();
                    QString urlVideoVODRecord = liveToVod(source, timeRangeRecord);
                    TypeTime tmp_TypeTime;
                    tmp_TypeTime = GeneralDefine::instance().convertSecondsToTexts(
                                mTimeStartRecordVOD);
                    QString dateFile =
                            tmp_TypeTime.day.append("_").append(tmp_TypeTime.time);
                    QString fileNameChange = "Video_" + nameCamera + "_" + dateFile;
                    fileNameChange.replace("/", "_");
                    fileNameChange.replace(":", ".");

                    InfomationRecordVOD *infoVideoRecord = new InfomationRecordVOD;

                    infoVideoRecord->url = urlVideoVODRecord;
                    infoVideoRecord->filename = fileNameChange;

                    fetchPathRecordVODRecord(urlVideoVODRecord, "namrtc", infoVideoRecord);

                    //start send to socket getclip

                    //                    QVariant *dataStruct = new QVariant();
                    //                    DataGetClip dataClipRecord;
                    //                    dataClipRecord.cameraId = camItemSelected->getCameraId();
                    //                    dataClipRecord.nameVideo = fileNameChange;
                    //                    dataClipRecord.urlVideo = urlVideoVODRecord;
                    //                    dataStruct->setValue<DataGetClip>(dataClipRecord);
                    //                    control()->newUserAction(Message.APP_PLAY_BACK_GET_CLIP_RECORD, dataStruct);
                }
            }
        }
    }
}

void P_PBWorkSpace::recordVideoQuick(TimeStampRecord timeRecord) {
    isRecordDefault = false;
    TimeStampRecord mTimeRecord;
    mTimeRecord = timeRecord;
    long secondRecordMore =
            mTimeRecord.endTimeStampRecord - mTimeRecord.startTimeStampRecord;
    CamItem *camItemSelected = pageCameras.at(selectedIndex);
    qDebug() << Q_FUNC_INFO << pageCameras.size()
             << camItemSelected->getPostion();
    QString cameraId;
    if (camItemSelected) {
        if (camItemSelected->isTv()) {
            // namtv record quick
            cameraId = QString::number(camItemSelected->getCameraId());

            TimeRange timeRangeRecord;
            timeRangeRecord.starttime = mTimeRecord.startTimeStampRecord;
            timeRangeRecord.length = secondRecordMore;

            QString nameCamera = camItemSelected->getPostion();

            QString urlVideoVODRecord;

            fetchVideoURLVODRecord(cameraId, timeRangeRecord);

            if (mVersionApi == "2.0") {
                urlVideoVODRecord =
                        this->mVideoVODRecord->getVideoVODProfile()->getSrc();

            } else if (mVersionApi == "3.0") {
                urlVideoVODRecord = this->mVideoVODRecord->getSrcMain();
            }

            TypeTime tmp_TypeTime;
            tmp_TypeTime = GeneralDefine::instance().convertSecondsToTexts(
                        mTimeRecord.startTimeStampRecord);
            QString dateFile = tmp_TypeTime.day.append("_").append(tmp_TypeTime.time);
            QString fileNameChange = "Video_" + nameCamera + "_" + dateFile;
            fileNameChange.replace("/", "_");
            fileNameChange.replace(":", ".");

            InfomationRecordVOD *infoVideoRecord = new InfomationRecordVOD;
            infoVideoRecord->url = urlVideoVODRecord;
            infoVideoRecord->filename = fileNameChange;

            fetchPathRecordVODRecord(urlVideoVODRecord, "namtv", infoVideoRecord);

        } else {
            // namrtc record quick

            cameraId = QString::number(camItemSelected->getCameraId());
            QString nameCamera = camItemSelected->getPostion();

            TimeRange timeRangeRecord;
            timeRangeRecord.starttime = mTimeRecord.startTimeStampRecord;
            timeRangeRecord.length = secondRecordMore;

            qDebug() << "Time start record" << timeRangeRecord.starttime << " length"
                     << timeRangeRecord.length;

            // de lay link record theo luong thoi gian truyen vao

            C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                        this->control(), Q_NULLPTR, appName, QString::number(selectedIndex));
            CamItemType type;
            type.name = cPlayer->getModePlayerCurrent();

            // de lay link record theo luong thoi gian truyen vao
            CamStream *camStream = camItemSelected->getCamStream(type);
            if(camStream){
                QString source = camStream->getSource();
                QString urlVideoVODRecord = liveToVod(source, timeRangeRecord);
                TypeTime tmp_TypeTime;
                tmp_TypeTime = GeneralDefine::instance().convertSecondsToTexts(
                            mTimeStartRecordVOD);
                QString dateFile =
                        tmp_TypeTime.day.append("_").append(tmp_TypeTime.time);
                QString fileNameChange = "Video_" + nameCamera + "_" + dateFile;
                fileNameChange.replace("/", "_");
                fileNameChange.replace(":", ".");

                InfomationRecordVOD *infoVideoRecord = new InfomationRecordVOD;

                infoVideoRecord->url = urlVideoVODRecord;
                infoVideoRecord->filename = fileNameChange;
                fetchPathRecordVODRecord(urlVideoVODRecord, "namrtc", infoVideoRecord);

                //start send to socket getclip

                //                QVariant *dataStruct = new QVariant();
                //                DataGetClip dataClipRecord;
                //                dataClipRecord.cameraId = camItemSelected->getCameraId();
                //                dataClipRecord.nameVideo = fileNameChange;
                //                dataClipRecord.urlVideo = urlVideoVODRecord;
                //                dataStruct->setValue<DataGetClip>(dataClipRecord);
                //                control()->newUserAction(Message.APP_PLAY_BACK_GET_CLIP_RECORD, dataStruct);
            }
        }
    }
}

void P_PBWorkSpace::startDownLoadVideoRecord(QString urlFileMp4Record){
    //if record default
    if (!urlFileMp4Record.isNull()) {
        if(isRecordDefault){
            DownloadManager download(this->zone);
            connect(&download, &DownloadManager::cancelSaveVideoRecord, this,
                    &P_PBWorkSpace::cancelSaveVideoRecord);
            connect(&download, &DownloadManager::linkRecordNotFound, this,
                    &P_PBWorkSpace::linkRecordNotFound);
            download.startDownloading(urlFileMp4Record, this->zone);
        }else{
            DownloadManager download(this->zone);
            connect(&download, &DownloadManager::linkRecordNotFound, this,
                    &P_PBWorkSpace::linkRecordNotFound);
            connect(&download, &DownloadManager::downloadVideoSuccess, this,
                    &P_PBWorkSpace::downloadVideoRecordSuccess);
            connect(&download, &DownloadManager::cancelSaveVideoRecord, this,
                    &P_PBWorkSpace::cancelSaveVideoRecord);
            connect(&download, &DownloadManager::percentProcessbar, this,
                    &P_PBWorkSpace::updateProcessBar);
            download.startDownloading(urlFileMp4Record, this->zone);
        }
    }
}



void P_PBWorkSpace::updateProcessBar(int percent) {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<int>(percent);
    control()->newUserAction(Message.APP_PLAY_BACK_UPDATE_PROCESS_BAR,
                             dataStruct);
}

void P_PBWorkSpace::cancelSaveVideoRecord() {
    control()->newUserAction(Message.APP_PLAY_BACK_CANCEL_SAVE_VIDEO, Q_NULLPTR);
}
void P_PBWorkSpace::downloadVideoRecordSuccess() {
    control()->newUserAction(Message.APP_PLAY_BACK_DOWNLOAD_VIDEO_SUCCESS,
                             Q_NULLPTR);
}
void P_PBWorkSpace::linkRecordNotFound() {
    control()->newUserAction(Message.APP_PLAY_BACK_LINK_RECORD_NOT_FOUND,
                             Q_NULLPTR);
}

void P_PBWorkSpace::fetchVideoURLVODRecord(QString cameraId,
                                           TimeRange timeRangeRequest) {
    function<void(QString)> onFailure = [this](QString message) {

    };

    function<void(VideoVOD *)> onSuccess = [this](VideoVOD *videoVOD) {
        this->setVideoVODRecord(videoVOD);
    };
    QString userToken = control()->appContext->getWorkingUser()->getToken();

    VideoVOD::fetchOneVideoVOD(userToken, cameraId, timeRangeRequest, onSuccess,
                               onFailure);
}

void P_PBWorkSpace::setVideoVODRecord(VideoVOD *videoVOD) {
    mVideoVODRecord = videoVOD;
}

QString P_PBWorkSpace::convertPathVODToUrlVideoMP4(QString url,
                                                   QString fileName) {
    QString http = url.split("://").first();
    QString uri = url.split("://").last();
    QString uriBase = http.append("://").append(uri.split("/").first());
    QString pathRecord = fileName.split("/html").last();
    return uriBase.append(pathRecord);
}

void P_PBWorkSpace::fetchPathRecordVODRecord(
        QString url, QString type, InfomationRecordVOD *infoRecodeVOD) {
    function<void(QString)> onFailure = [this](QString message) {

    };

    function<void(RecordVideoVOD *)> onSuccess =
            [this, url](RecordVideoVOD *recordVideoVOD) {
        QString urlDownloadFileMp4 = convertPathVODToUrlVideoMP4(url, recordVideoVOD->getPath());
        seturlVideoRecord(urlDownloadFileMp4);
        startDownLoadVideoRecord(urlDownloadFileMp4);
    };

    if (type == "namtv") {
        RecordVideoVOD::fetchVideoRecordVODNamTv(infoRecodeVOD, onSuccess,
                                                 onFailure);
    } else if (type == "namrtc") {
        RecordVideoVOD::fetchVideoRecordVODNamRTC(infoRecodeVOD, onSuccess,
                                                  onFailure);
    }
}

void P_PBWorkSpace::seturlVideoRecord(QString url) { mUrlVideoRecord = url; }

void P_PBWorkSpace::startAnimationRecordNormalButton() {
    C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                control(), Q_NULLPTR, appName, QString::number(selectedIndex));
    cPlayer->newAction(Message.APP_PLAY_BACK_START_ANIMAITON_RECORD_NORMAL,
                       Q_NULLPTR);
}
void P_PBWorkSpace::stopAnimationRecordNormalButton() {
    C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                control(), Q_NULLPTR, appName, QString::number(selectedIndex));
    cPlayer->newAction(Message.APP_PLAY_BACK_STOP_ANIMAITON_RECORD_NORMAL,
                       Q_NULLPTR);
}
// dùng cho seek bar
void P_PBWorkSpace::seekbarToNewPosition(QVariant *dataStruct) {
    for (int index = 0; index < pageCameras.size(); ++index) {
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    this->control(), Q_NULLPTR, appName, QString::number(index));
        cPlayer->newAction(Message.APP_PLAY_BACK_SEEK_BAR_TO_NEW_POSITION,
                           dataStruct);
    }
}



void P_PBWorkSpace::resizePlayerWhenExitFullScreen(){
    //neu van dang full screen  thi resize player
    if(selectedIndex != - 1){
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        cPlayer->newAction(Message.EXIT_FULLSCREEN_MODE, Q_NULLPTR);
    }

}

void P_PBWorkSpace::resizePlayerWhenEnterFullScreen(){
    //neu van dang full screen  thi resize player
    if(selectedIndex != - 1){
        C_Cam9Player *cPlayer = Cam9PlayerPool::instance().getPlayer(
                    control(), Q_NULLPTR, appName, QString::number(selectedIndex));
        cPlayer->newAction(Message.ENTER_FULLSCREEN_MODE, Q_NULLPTR);
    }

}
