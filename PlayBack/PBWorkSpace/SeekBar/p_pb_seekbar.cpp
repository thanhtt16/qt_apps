#include "p_pb_seekbar.h"

/**
     * Generic method to override for updating the presention.
     **/
P_PBSeekBar::P_PBSeekBar(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    // init gui object
    zone->setStyleSheet("background-color: #ecf0f1");
    // init gui object
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignCenter);
    zone->setLayout(layout);
    mBottomLeftWidget = new QWidget(zone);
    mBottomRightWidget = new QWidget(zone);
    layout->addWidget(mBottomLeftWidget);
    layout->addWidget(mBottomRightWidget);

    initLeft();
    initRight();

    mVersion = AppProfile::getAppProfile()->getAppConfig()->getApiVodVersion();
}

void P_PBSeekBar::update() {}

QObject *P_PBSeekBar::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}

void P_PBSeekBar::initRight() {
    mBottomRightLayout = new QHBoxLayout();
    mBottomRightLayout->setContentsMargins(10, 0, 10, 0);
    mBottomRightLayout->setSpacing(0);
    mBottomRightLayout->setAlignment(Qt::AlignCenter);
    mBottomRightWidget->setLayout(mBottomRightLayout);
    initBotBackWard();
}

void P_PBSeekBar::sliderChangeValue(int value) {
    if (mIsSliderPressed != 1) {
        lastValueSlider = currentValueSlider;
    }
    currentValueSlider = value;  // cap nhat lai gia tri mois
}
// khi click vao
void P_PBSeekBar::onPressSlider() {
    mIsSliderPressed = 1;  // neu click thi danh dau no da duoc click
}

void P_PBSeekBar::initLeft() {
    // init bottom left layout
    mBottomLeftLayout = new QVBoxLayout();
    mBottomLeftLayout->setContentsMargins(10, 0, 10, 0);
    mBottomLeftLayout->setSpacing(10);
    mBottomLeftLayout->setAlignment(Qt::AlignCenter);
    mBottomLeftWidget->setLayout(mBottomLeftLayout);

    // init slider
    mVideoSlider = new QSlider(mBottomLeftWidget);
    mVideoSlider->setRange(0, lengthVideoVOD);
    mVideoSlider->setOrientation(Qt::Horizontal);
    mBottomLeftLayout->addWidget(mVideoSlider);
    connect(mVideoSlider, &QSlider::valueChanged, this,
            &P_PBSeekBar::sliderChangeValue);
    connect(mVideoSlider, &QSlider::sliderReleased, this,
            &P_PBSeekBar::onReleaseSlider);
    connect(mVideoSlider, &QSlider::sliderPressed, this,
            &P_PBSeekBar::onPressSlider);
    mUpdateSliderTimer = new QTimer(this);
    connect(mUpdateSliderTimer, &QTimer::timeout, this,
            &P_PBSeekBar::updateValueSlider);
}
void P_PBSeekBar::initBotBackWard() {
    QWidget *mWidgetBackWard = new QWidget(mBottomRightWidget);
    QHBoxLayout *mLayoutBackWard = new QHBoxLayout();
    mLayoutBackWard->setSpacing(1);
    mLayoutBackWard->setContentsMargins(0, 0, 10, 0);
    mWidgetBackWard->setLayout(mLayoutBackWard);

    mBackWardButton = new QPushButton(mWidgetBackWard);
    mBackWardButton->setText(iconButtonTops.backward);
    mBackWardButton->setStyleSheet(
                "background-color: #E6E9EA; color: #BEBEBE; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 5px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 5px;");
    mBackWardButton->setFixedSize(40, 30);
    mBackWardButton->setEnabled(false);
    connect(mBackWardButton, &QPushButton::clicked, this,
            &P_PBSeekBar::selectedBackWard);

    mForWardButton = new QPushButton(mWidgetBackWard);
    mForWardButton->setText(iconButtonTops.forward);
    mForWardButton->setStyleSheet(
                "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                "border-bottom-left-radius: 0px;");
    mForWardButton->setFixedSize(40, 30);
    connect(mForWardButton, &QPushButton::clicked, this,
            &P_PBSeekBar::selectedForWard);

    mPauseButton = new QPushButton(mWidgetBackWard);
    mPauseButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0x; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius:0px;");
    mPauseButton->setText(iconButtonTops.pause);
    mPauseButton->setFixedSize(40, 30);
    connect(mPauseButton, &QPushButton::clicked, this,
            &P_PBSeekBar::pauseVideoVOD);

    mLayoutBackWard->addWidget(mBackWardButton);
    mLayoutBackWard->addWidget(mPauseButton);
    mLayoutBackWard->addWidget(mForWardButton);

    QWidget *mWidgetBackWardButton = new QWidget(mBottomRightWidget);
    QHBoxLayout *mLayoutBackWardButton = new QHBoxLayout();
    mWidgetBackWardButton->setLayout(mLayoutBackWardButton);
    mLayoutBackWardButton->setAlignment(Qt::AlignHCenter);
    mLayoutBackWardButton->setSpacing(1);
    mLayoutBackWardButton->setMargin(0);

    mForwardByFrameButton = new QPushButton(mBottomRightWidget);
    QPixmap pixmapbyframe(":/images/res/iconbyframe.png");
    QIcon iconbyframe(pixmapbyframe);
    mForwardByFrameButton->setIcon(iconbyframe);

    mForwardByFrameButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 5px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 5px;");

    mForwardByFrameButton->setFixedSize(40, 30);
    connect(mForwardByFrameButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forwardByFrame);

    mForWard025xButton = new QPushButton(mWidgetBackWardButton);
    QPixmap pixmap025x(":/images/res/icon025x.png");
    QIcon icon025x(pixmap025x);
    mForWard025xButton->setIcon(icon025x);

    mForWard025xButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 0px;");
    mForWard025xButton->setFixedSize(40, 30);
    connect(mForWard025xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed025x);

    mForWard05xButton = new QPushButton(mWidgetBackWardButton);
    QPixmap pixmap05x(":/images/res/icon05x.png");
    QIcon icon05x(pixmap05x);
    mForWard05xButton->setIcon(icon05x);
    connect(mForWard05xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed05x);

    //    mForWard05xButton->setText("0.5x");

    mForWard05xButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 0px;");
    mForWard05xButton->setFixedSize(40, 30);

    mForWard1xButton = new QPushButton(mWidgetBackWardButton);
    connect(mForWard1xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed1x);
    mForWard1xButton->setText("1x");
    // selected
    mForWard1xButton->setStyleSheet(
                "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 0px;");
    mForWard1xButton->setFixedSize(40, 30);

    mForWard2xButton = new QPushButton(mWidgetBackWardButton);
    connect(mForWard2xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed2x);

    mForWard2xButton->setText("2x");

    mForWard2xButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 0px;");
    mForWard2xButton->setFixedSize(40, 30);

    mForWard4xButton = new QPushButton(mWidgetBackWardButton);
    connect(mForWard4xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed4x);

    mForWard4xButton->setText("4x");
    mForWard4xButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 0px;");
    mForWard4xButton->setFixedSize(40, 30);

    mForWard8xButton = new QPushButton(mWidgetBackWardButton);
    connect(mForWard8xButton, &QPushButton::clicked, this,
            &P_PBSeekBar::forWardSpeed8x);
    mForWard8xButton->setText("8x");
    mForWard8xButton->setStyleSheet(
                "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                "border-bottom-left-radius: 0px;");
    mForWard8xButton->setFixedSize(40, 30);

    mLayoutBackWardButton->addWidget(mForwardByFrameButton);
    mLayoutBackWardButton->addWidget(mForWard025xButton);
    mLayoutBackWardButton->addWidget(mForWard05xButton);
    mLayoutBackWardButton->addWidget(mForWard1xButton);
    mLayoutBackWardButton->addWidget(mForWard2xButton);
    mLayoutBackWardButton->addWidget(mForWard4xButton);
    mLayoutBackWardButton->addWidget(mForWard8xButton);
    //    mLayoutBackWardButton->addWidget(mForWard16xButton);

    mBottomRightLayout->addWidget(mWidgetBackWard);
    mBottomRightLayout->addWidget(mWidgetBackWardButton);
}

void P_PBSeekBar::onReleaseSlider() {
    if (mIsShowOneVideo == 0) {
        int secondMove =
                qFabs(currentValueSlider - lastValueSlider);  // so giay di chuyen
        qDebug() << "so giay di chuyen" << secondMove;
        if ((currentValueSlider > lastValueSlider) &&
                (currentValueSlider <= lengthVideoVOD))  // neu sau lon hon truoc
        {
            mStartTimePlayVideoAllVOD = mStartTimePlayVideoAllVOD +
                    secondMove;  // thoi gian start cap nhat lai
            mlenghtPlayVideoAllVOD =
                    lengthVideoVOD - currentValueSlider;  // lay lai do dai cua video
            TimeRange tmpTimeRangeRequest;
            tmpTimeRangeRequest.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRangeRequest.length = mlenghtPlayVideoAllVOD;
            setTimeRangeRequestVOD(tmpTimeRangeRequest);
            playListVideoVOD(tmpTimeRangeRequest);
            mIsSliderPressed = 0;
        } else if (currentValueSlider <= lengthVideoVOD) {
            mStartTimePlayVideoAllVOD = mStartTimePlayVideoAllVOD -
                    secondMove;  // thoi gian start cap nhat lai
            mlenghtPlayVideoAllVOD =
                    lengthVideoVOD - currentValueSlider;  // lay lai do dai cua video
            TimeRange tmpTimeRangeRequest;
            tmpTimeRangeRequest.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRangeRequest.length = mlenghtPlayVideoAllVOD;
            // set gia tri
            setTimeRangeRequestVOD(tmpTimeRangeRequest);
            playListVideoVOD(tmpTimeRangeRequest);
            mIsSliderPressed = 0;
        }
    } else {
        qDebug() << "relase for one video";
        int secondMove =
                qFabs(currentValueSlider - lastValueSlider);  // so giay di chuyen
        qDebug() << "so giay di chuyen" << secondMove;
        if ((currentValueSlider > lastValueSlider) &&
                (currentValueSlider <= lengthVideoVOD))  // neu sau lon hon truoc
        {
            mStartTimePlayVideoAllVOD = mStartTimePlayVideoAllVOD +
                    secondMove;  // thoi gian start cap nhat lai
            mlenghtPlayVideoAllVOD =
                    lengthVideoVOD - currentValueSlider;  // lay lai do dai cua video
            TimeRange tmpTimeRangeRequest;
            tmpTimeRangeRequest.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRangeRequest.length = mlenghtPlayVideoAllVOD;
            // set gia tri
            setTimeRangeRequestVOD(tmpTimeRangeRequest);
            playListVideoVOD(tmpTimeRangeRequest);
            mIsSliderPressed = 0;

        } else if (currentValueSlider <= lengthVideoVOD) {
            qDebug() << "request url sau khi buong2";

            mStartTimePlayVideoAllVOD = mStartTimePlayVideoAllVOD -
                    secondMove;  // thoi gian start cap nhat lai
            mlenghtPlayVideoAllVOD =
                    lengthVideoVOD - currentValueSlider;  // lay lai do dai cua video
            TimeRange tmpTimeRangeRequest;
            tmpTimeRangeRequest.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRangeRequest.length = mlenghtPlayVideoAllVOD;
            // set gia tri
            setTimeRangeRequestVOD(tmpTimeRangeRequest);
            playListVideoVOD(tmpTimeRangeRequest);
            mIsSliderPressed = 0;
        }
    }
}

void P_PBSeekBar::setTimeRangeRequestVOD(TimeRange timeRange) {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<TimeRange>(timeRange);
    control()->newUserAction(Message.APP_PLAY_BACK_UPDATE_TIME_STAMP_CURRENT,
                             dataStruct);
}

// moi giay cap nhat lai gia tri cho slider
void P_PBSeekBar::updateValueSlider() {
    if (mIsShowOneVideo == 0) {
        if ((mIsSliderPressed != 1) && (mVideoSlider->value() <= lengthVideoVOD)) {
            mVideoSlider->setValue(mVideoSlider->value() + mSpeedAllVideo);
            mlenghtPlayVideoAllVOD = mlenghtPlayVideoAllVOD - 1;
            mStartTimePlayVideoAllVOD = mStartTimePlayVideoAllVOD + 1;
            TimeRange tmpTimeRange;
            tmpTimeRange.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRange.length = mlenghtPlayVideoAllVOD;
            setTimeRangeRequestVOD(tmpTimeRange);
        }
    } else {
        // truong hop mot video
        if ((mIsSliderPressed != 1) && (mVideoSlider->value() <= lengthVideoVOD)) {
            mVideoSlider->setValue(mVideoSlider->value() + mSpeedAllVideo);
            mStartTimePlayVideoAllVOD =
                    mStartTimePlayVideoAllVOD + mSpeedAllVideo;  // moi giay thi tang them
            mlenghtPlayVideoAllVOD = mlenghtPlayVideoAllVOD - 1;
            // set lai thoi gian lien tuc
            TimeRange tmpTimeRange;
            tmpTimeRange.starttime = mStartTimePlayVideoAllVOD;
            tmpTimeRange.length = mlenghtPlayVideoAllVOD;
            setTimeRangeRequestVOD(tmpTimeRange);
        }
    }
}
void P_PBSeekBar::playListVideoVOD(TimeRange timeRange) {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<TimeRange>(timeRange);
    control()->newUserAction(Message.APP_PLAY_BACK_PLAY_LIST_VIDEO_VOD,
                             dataStruct);
}
void P_PBSeekBar::playOneVideoVOD(TimeRange timeRange) {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<TimeRange>(timeRange);
    control()->newUserAction(Message.APP_PLAY_BACK_PLAY_ONE_VIDEO_VOD,
                             dataStruct);
}

void P_PBSeekBar::seekToNewPostion(long newPostion) {
    mVideoSlider->setValue(0);
    mStartTimePlayVideoAllVOD = newPostion;
    mLastTimeStampSelect = newPostion;
}

void P_PBSeekBar::forwardByFrame() {
    setBackgroundPlayBack(true, false, false, false, false, false, false, false);
    mSpeedAllVideo = 0.1f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed025x() {
    setBackgroundPlayBack(false, true, false, false, false, false, false, false);
    mSpeedAllVideo = 0.25f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed05x() {
    setBackgroundPlayBack(false, false, true, false, false, false, false, false);
    mSpeedAllVideo = 0.5f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed1x() {
    setBackgroundPlayBack(false, false, false, true, false, false, false, false);
    mSpeedAllVideo = 1.0f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed2x() {
    setBackgroundPlayBack(false, false, false, false, true, false, false, false);
    mSpeedAllVideo = 2.0f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed4x() {
    setBackgroundPlayBack(false, false, false, false, false, true, false, false);
    mSpeedAllVideo = 4.0f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::forWardSpeed8x() {
    setBackgroundPlayBack(false, false, false, false, false, false, true, false);
    mSpeedAllVideo = 8.0f;
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<double>(mSpeedAllVideo);
    changedSpeedAllVideo(dataStruct);
}
void P_PBSeekBar::selectedBackWard() {
    mBackWardButton->setStyleSheet(
                "background-color: #E6E9EA; color: #BEBEBE; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 5px; "
                "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                "border-bottom-left-radius: 5px;");
}
void P_PBSeekBar::selectedForWard() {
    mForWardButton->setStyleSheet(
                "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                "border-style: solid; border-top-left-radius: 0px; "
                "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                "border-bottom-left-radius: 0px;");
}

void P_PBSeekBar::setBackgroundPlayBack(bool isByFrame, bool is025x, bool is05x,
                                        bool is1x, bool is2x, bool is4x,
                                        bool is8x, bool is16x) {
    if (isByFrame) {
        //        mForwardByFrameButton->setStyleSheet("background-color: #6d6d6c;
        //        color: #f7f7f7;");
        QPixmap pixmapbyframe(":/images/res/iconbyframeselected.png");
        QIcon iconbyframe(pixmapbyframe);
        mForwardByFrameButton->setIcon(iconbyframe);
    } else {
        //        mForwardByFrameButton->setStyleSheet("background-color: #f7f7f7;
        //        color: #6d6d6c;");
        QPixmap pixmapbyframe(":/images/res/iconbyframe.png");
        QIcon iconbyframe(pixmapbyframe);
        mForwardByFrameButton->setIcon(iconbyframe);
    }

    if (is025x) {
        QPixmap pixmap025x(":/images/res/icon025xselected.png");
        QIcon icon025x(pixmap025x);
        mForWard025xButton->setIcon(icon025x);
        //        mForWard025xButton->setStyleSheet("background-color: #6d6d6c;
        //        color: #f7f7f7; border-width: 0px;  border-style: solid;
        //        border-top-left-radius: 0px; border-top-right-radius: 0px;
        //        border-bottom-right-radius: 0px; border-bottom-left-radius:
        //        0px;");
    } else {
        QPixmap pixmap025x(":/images/res/icon025x.png");
        QIcon icon025x(pixmap025x);
        mForWard025xButton->setIcon(icon025x);
        //        mForWard025xButton->setStyleSheet("background-color: #f7f7f7;
        //        color: #6d6d6c; border-width: 0px;  border-style: solid;
        //        border-top-left-radius: 0px; border-top-right-radius: 0px;
        //        border-bottom-right-radius: 0px; border-bottom-left-radius:
        //        0px;");
    }
    if (is05x) {
        QPixmap pixmap05x(":/images/res/icon05xselected.png");
        QIcon icon05x(pixmap05x);
        mForWard05xButton->setIcon(icon05x);
        //        mForWard05xButton->setStyleSheet("background-color: #6d6d6c;
        //        color: #f7f7f7; border-width: 0px;  border-style: solid;
        //        border-top-left-radius: 0px; border-top-right-radius: 0px;
        //        border-bottom-right-radius: 0px; border-bottom-left-radius:
        //        0px;");
    } else {
        QPixmap pixmap05x(":/images/res/icon05x.png");
        QIcon icon05x(pixmap05x);
        mForWard05xButton->setIcon(icon05x);
        // mForWard05xButton->setStyleSheet("background-color: #f7f7f7; color:
        // #6d6d6c; border-width: 0px;  border-style: solid; border-top-left-radius:
        // 0px; border-top-right-radius: 0px; border-bottom-right-radius: 0px;
        // border-bottom-left-radius: 0px;");
    }

    if (is1x) {
        mForWard1xButton->setStyleSheet(
                    "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    } else {
        mForWard1xButton->setStyleSheet(
                    "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    }
    if (is2x) {
        mForWard2xButton->setStyleSheet(
                    "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    } else {
        mForWard2xButton->setStyleSheet(
                    "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    }
    if (is4x) {
        mForWard4xButton->setStyleSheet(
                    "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    } else {
        mForWard4xButton->setStyleSheet(
                    "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 0px; border-bottom-right-radius: 0px; "
                    "border-bottom-left-radius: 0px;");
    }

    if (mIsShowOneVideo == 0) {

        if (layoutCurrent < 16) {
            mForWard8xButton->setDisabled(false);
            if (is8x) {
                mForWard8xButton->setStyleSheet(
                            "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                            "border-style: solid; border-top-left-radius: 0px; "
                            "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                            "border-bottom-left-radius: 0px;");
            } else {
                mForWard8xButton->setStyleSheet(
                            "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                            "border-style: solid; border-top-left-radius: 0px; "
                            "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                            "border-bottom-left-radius: 0px;");
            }
        } else {
            mForWard8xButton->setDisabled(true);
            mForWard8xButton->setStyleSheet(
                        "background-color: #a1a1a1; color: #8e8e8e; border-width: 0px;  "
                        "border-style: solid; border-top-left-radius: 0px; "
                        "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                        "border-bottom-left-radius: 0px;");
        }

    } else {
        if (is8x) {
            //enable and seleceted
            mForWard8xButton->setStyleSheet(
                        "background-color: #6d6d6c; color: #f7f7f7; border-width: 0px;  "
                        "border-style: solid; border-top-left-radius: 0px; "
                        "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                        "border-bottom-left-radius: 0px;");
        } else {
            //enable not selected
            mForWard8xButton->setStyleSheet(
                        "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                        "border-style: solid; border-top-left-radius: 0px; "
                        "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                        "border-bottom-left-radius: 0px;");
        }
    }
}

void P_PBSeekBar::pauseVideoVOD() {}

void P_PBSeekBar::changedSpeedAllVideo(QVariant *dataStruct) {
    control()->newUserAction(Message.APP_PLAY_BACK_CHANGE_SPEED_ALL_VIDEO,
                             dataStruct);
}

void P_PBSeekBar::resetPlayerSpeed() {
    mSpeedAllVideo = 1.0f;
    setBackgroundPlayBack(false, false, false, true, false, false, false, false);
}

void P_PBSeekBar::resetValueSeekBar() {
    mVideoSlider->setValue(0);
}
void P_PBSeekBar::resetValueWhenAppShow(){
    if(!mUpdateSliderTimer->isActive()){
        mUpdateSliderTimer->start(1000);
        mVideoSlider->setValue(0);
    }
}
void P_PBSeekBar::stopWhenChangeApp(){
    if(mUpdateSliderTimer->isActive()){
        mUpdateSliderTimer->stop();
        mVideoSlider->setValue(0);
        TimeRange timeRangeLast;
        mStartTimePlayVideoAllVOD = mLastTimeStampSelect;
        timeRangeLast.starttime = mLastTimeStampSelect;
        timeRangeLast.length = 3600;
        setTimeRangeRequestVOD(timeRangeLast);
    }
}

void P_PBSeekBar::disableSpeedButton(LayoutStruct layout) {
    layoutCurrent = layout.numberOfCameras;
}

void P_PBSeekBar::enableSpeedButton() {
    mIsShowOneVideo = 1;
    mForWard8xButton->setEnabled(true);
    if(layoutCurrent >= 16){
        mForWard8xButton->setStyleSheet(
                    "background-color: #f7f7f7; color: #6d6d6c; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                    "border-bottom-left-radius: 0px;");
    }
}

void P_PBSeekBar::disableSpeedButton() {
    mIsShowOneVideo = 0;
    mForWard8xButton->setDisabled(true);
    if(layoutCurrent >= 16){
        mForWard8xButton->setStyleSheet(
                    "background-color: gray; color: #fff; border-width: 0px;  "
                    "border-style: solid; border-top-left-radius: 0px; "
                    "border-top-right-radius: 5px; border-bottom-right-radius: 5px; "
                    "border-bottom-left-radius: 0px;");
        resetPlayerSpeed();
    }
}


