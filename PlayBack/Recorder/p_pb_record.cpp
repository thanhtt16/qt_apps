#include "p_pb_record.h"
/**
     * Generic method to override for updating the presention.
     **/

P_PBRecord::P_PBRecord(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
  // init gui object
  this->zone = zone;
  initUI();
}

void P_PBRecord::update() {}

QWidget *P_PBRecord::getZone(int zoneId) {
  switch (zoneId) {
    case 1:
      return Q_NULLPTR;
    default:
      return Q_NULLPTR;
  }
}

void P_PBRecord::initUI() {
  mainVLayout = new QVBoxLayout();
  mainVLayout->setMargin(0);
  mainVLayout->setSpacing(0);
  this->zone->setLayout(mainVLayout);

  pPanelWidget = new QWidget(this->zone);
  pPanelLayout = new QVBoxLayout();
  pPanelLayout->setSpacing(0);
  pPanelLayout->setMargin(0);
  pPanelWidget->setLayout(pPanelLayout);
  pPanelWidget->setStyleSheet("background:#222");
  mainVLayout->addWidget(pPanelWidget);

  mMainTabWidget = new QTabWidget(pPanelWidget);
  mMainTabWidget->setStyleSheet("QTabBar::tab:selected{background:#ff6e6e}");

  mMainTabWidget->setFont(Resources::instance().getLargeBoldButtonFont());
  QFont fontTab = mMainTabWidget->font();
  fontTab.setPointSize(13);
  fontTab.setBold(true);
  mMainTabWidget->setFont(fontTab);
  pPanelLayout->addWidget(mMainTabWidget);

  connect(mMainTabWidget, &QTabWidget::currentChanged, this,
          &P_PBRecord::onChangeTabIndex);
  initLeftWidget();
  initRightWidget();
  mMainTabWidget->setCurrentIndex(0);
  initAllBotDateWidget();
  initBotWidget();
}
void P_PBRecord::initBotWidget() {
  botWidget = new QWidget(pPanelWidget);
  botVBoxLayout = new QVBoxLayout();
  botVBoxLayout->setAlignment(Qt::AlignCenter);
  botVBoxLayout->setSpacing(0);
  botVBoxLayout->setMargin(0);
  botWidget->setLayout(botVBoxLayout);

  botWidget->setFixedSize(pPanelWidget->maximumWidth(), mHeightBotWidget);
  pPanelLayout->addWidget(botWidget);

  // add controll
  botControlWidget = new QWidget(botWidget);
  botControlWidget->setStyleSheet("background-color:#222; color #ffffff");
  pCloseButton = new QPushButton(botControlWidget);

  pCloseButton->setText("Đóng");
  pCloseButton->setFixedSize(150, 40);
  pCloseButton->setFont(Resources::instance().getLargeBoldButtonFont());
  QFont fontLableClose = pCloseButton->font();
  fontLableClose.setPointSize(13);
  fontLableClose.setBold(true);
  pCloseButton->setFont(fontLableClose);

  connect(pCloseButton, &QPushButton::clicked, this,
          &P_PBRecord::closeMenuRecord);

  mButtonStartRecord = new QPushButton(botControlWidget);
  mButtonStartRecord->setText(tr("Chiết xuất"));
  mButtonStartRecord->setFixedSize(150, 40);

  mButtonStartRecord->setFont(Resources::instance().getLargeBoldButtonFont());
  mButtonStartRecord->setFont(fontLableClose);

  connect(mButtonStartRecord, &QPushButton::clicked, this,
          &P_PBRecord::startRecordDetail);

  botControlWidget->setFixedHeight(50);
  botControlHBoxLayout = new QHBoxLayout();
  botControlWidget->setLayout(botControlHBoxLayout);

  botControlHBoxLayout->setAlignment(Qt::AlignHCenter);
  botControlHBoxLayout->addWidget(mButtonStartRecord);
  botControlHBoxLayout->setSpacing(5);
  //    botControlHBoxLayout->addWidget(mRecordDefalt);
  botControlHBoxLayout->addWidget(pCloseButton);
  botVBoxLayout->addWidget(botControlWidget);

  // add processbar
  botProcesserWidget = new QWidget(botWidget);
  botProcesserVBoxLayout = new QVBoxLayout();
  botProcesserVBoxLayout->setSpacing(5);
  botProcesserVBoxLayout->setMargin(5);

  botProcesserWidget->setFixedHeight(40);
  botProcesserVBoxLayout->setAlignment(Qt::AlignCenter);
  botProcesserWidget->setLayout(botProcesserVBoxLayout);

  processBar = new QProgressBar(botProcesserWidget);
  processBar->setRange(0, 100);  // Let's say it goes from 0 to 100
  processBar->setValue(0);       // With a current value of

  botProcesserVBoxLayout->addWidget(processBar);
  botVBoxLayout->addWidget(botProcesserWidget);
  botProcesserWidget->hide();
}
void P_PBRecord::onChangeTabIndex(int index) {
  mMainTabWidget->setCurrentIndex(index);
}

void P_PBRecord::initRightWidget() {
  // init title and calendar right
  rightWidget = new QWidget(mMainTabWidget);
  rightVBoxLayout = new QVBoxLayout();
  rightVBoxLayout->setAlignment(Qt::AlignTop);
  rightVBoxLayout->setSpacing(0);
  rightVBoxLayout->setMargin(0);
  rightWidget->setLayout(rightVBoxLayout);
  rightWidget->setFixedSize(mWidthRight, mHeigthRight);
  mMainTabWidget->addTab(rightWidget, "Kết thúc");

  rightWidget->setStyleSheet("background-color: #222");
  // init widget title
  titleEndWidget = new QWidget(rightWidget);
  rightVBoxLayout->addWidget(titleEndWidget);
  titleEndLayout = new QVBoxLayout();
  titleEndLayout->setSpacing(0);
  titleEndLayout->setMargin(0);
  titleEndWidget->setLayout(titleEndLayout);
  titleEndWidget->setFixedSize(mWidthRight, mHeigthTopWidget);
  //  titleEndWidget->setStyleSheet(
  //      utils.borderStyleSheet(colors.CLOUDS, colors.MIDNIGHT_BLUE, 0, 0));

  rightTitleLabel = new QLabel(titleEndWidget);
  rightTitleLabel->setText("Kết thúc");
  rightTitleLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  QFont fontLabelStart = rightTitleLabel->font();
  fontLabelStart.setPointSize(13);
  fontLabelStart.setBold(true);
  rightTitleLabel->setFont(fontLabelStart);
  rightTitleLabel->setAlignment(Qt::AlignCenter);
  titleEndLayout->addWidget(rightTitleLabel);

  // widget calendar
  calendarEndWidget = new QWidget(rightWidget);
  rightVBoxLayout->addWidget(calendarEndWidget);
  calendarEndWidget->setFixedSize(mWidthRight, mHeightCalendar);
  calendarEndLayout = new QVBoxLayout();
  calendarEndLayout->setSpacing(0);
  calendarEndLayout->setMargin(0);
  calendarEndWidget->setLayout(calendarEndLayout);
  // init calendar end
  calendarEnd = new QCalendarWidget(calendarEndWidget);
  calendarEnd->setStyleSheet(
      "QCalendarWidget QWidget { alternate-background-color: rgb(128, 128, "
      "128); }");

  calendarEndLayout->addWidget(calendarEnd);
  calendarEnd->setFont(Resources::instance().getLargeBoldButtonFont());
  QDate datecurrent = QDate::currentDate();
  calendarEnd->setSelectedDate(datecurrent);

  calendarEnd->setVerticalHeaderFormat(
      QCalendarWidget::VerticalHeaderFormat(QCalendarWidget::NoVerticalHeader));
  long unixtimeToDay = QDateTime::currentDateTime().toTime_t();
  long unixtimeLateHistoryday = unixtimeToDay - 29 * 24 * 60 * 60;
  QDateTime timeStampHistory;
  timeStampHistory.setTime_t(unixtimeLateHistoryday);
  QDate dateLate = timeStampHistory.date();
  calendarEnd->setMinimumDate(dateLate);
  calendarEnd->setMaximumDate(QDate::currentDate());
  // firstdaychanged
  calendarEnd->setFirstDayOfWeek(Qt::DayOfWeek(Qt::Monday));  // thu hai
  // hien thi tuan trong nam

  //    calendarEnd->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers));
  // format bold
  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);
  calendarEnd->setHeaderTextFormat(format);
  calendarEnd->setGridVisible(true);
}

void P_PBRecord::initAllBotDateWidget() {
  dateAllBotWidget = new QWidget(pPanelWidget);
  dateAllHBoxLayout = new QHBoxLayout();
  dateAllHBoxLayout->setAlignment(Qt::AlignCenter);

  dateAllHBoxLayout->setSpacing(0);
  dateAllHBoxLayout->setMargin(0);
  initDateBotLeftwidget();
  initDateBotRightWidget();

  dateAllBotWidget->setLayout(dateAllHBoxLayout);
  pPanelLayout->addWidget(dateAllBotWidget);
  //    dateAllBotWidget->setStyleSheet("background-color:#E4E4E4;color:#333");
  //    dateAllBotWidget->setStyleSheet(utils.simpleStyleSheet(colors.CLOUDS,colors.BLACK));
  dateAllBotWidget->setFixedSize(mWidthRight, mHeigthDateBot);

  // add righ and left date
  dateAllHBoxLayout->addWidget(dateStartWidget);
  dateAllHBoxLayout->addWidget(dateEndWidget);
}

void P_PBRecord::initDateBotLeftwidget() {
  // widget date Left
  dateStartWidget = new QWidget(dateAllBotWidget);
  dateStartLayout = new QVBoxLayout();
  dateStartLayout->setSpacing(5);
  dateStartWidget->setFixedSize(mWidthRight / 2, mHeigthDateBot);
  dateStartWidget->setLayout(dateStartLayout);

  dateStartEditLabel = new QLabel(dateStartWidget);
  dateStartEditLabel->setText(tr("Bắt đầu :"));
  QFont fontDateStart = dateStartEditLabel->font();
  fontDateStart.setPointSize(13);
  fontDateStart.setBold(true);
  dateStartEditLabel->setFont(fontDateStart);

  dateStartEdit = new QDateEdit(dateStartWidget);
  QString dateStartStylesheet =
      "QDateEdit { padding-left: 2px; border-width: 3;height : "
      "30px;font-size:13pt;color:#5858DC;background-color:#fff}";
  dateStartEdit->setStyleSheet(dateStartStylesheet);
  dateStartEdit->setFont(Resources::instance().getLargeBoldButtonFont());
  connect(dateStartEdit, &QDateEdit::dateChanged, calendarStart,
          &QCalendarWidget::setSelectedDate);
  connect(dateStartEdit, &QDateEdit::dateChanged, calendarStart,
          &QCalendarWidget::setSelectedDate);
  connect(calendarStart, &QCalendarWidget::selectionChanged, this,
          &P_PBRecord::selectedDateChangedStart);

  dateStartEdit->setDisplayFormat("dd-MM-yyyy");

  dateStartEdit->setDateRange(calendarStart->minimumDate(),
                              calendarStart->maximumDate());
  dateStartEdit->setDate(calendarStart->maximumDate());
  dateStartEditLabel->setBuddy(dateStartEdit);

  dateLeftGridLayout = new QGridLayout;
  dateLeftGridLayout->addWidget(dateStartEditLabel, 0, 2, 1, 3);
  dateLeftGridLayout->addWidget(dateStartEdit, 0, 4, 1, 3);
  dateStartLayout->addLayout(dateLeftGridLayout);

  QLabel *hourLabel = new QLabel(dateStartWidget);
  hourLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  hourLabel->setText("Giờ");
  //  QFont fontHourStart = hourLabel->font();
  //  fontHourStart.setPointSize(13);
  //  fontHourStart.setBold(true);
  //  hourLabel->setFont(fontHourStart);
  QLabel *minuteLabel = new QLabel(dateStartWidget);
  minuteLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  minuteLabel->setText("Phút");
  //  minuteLabel->setFont(fontHourStart);

  QLabel *secondLabel = new QLabel(dateStartWidget);
  secondLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  secondLabel->setText("Giây");
  //  secondLabel->setFont(fontHourStart);

  QString hourStylesheet =
      "QSpinBox { padding-Left: 2px; border-width: 3;height : "
      "30px;font-size:13pt;color:#5858DC;background-color:#fff}";
  hourStartSpinBox = new QSpinBox(dateStartWidget);
  hourStartSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  hourStartSpinBox->setStyleSheet(hourStylesheet);
  hourStartSpinBox->setRange(0, 23);

  minuteStartSpinBox = new QSpinBox(dateStartWidget);
  minuteStartSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  minuteStartSpinBox->setRange(0, 59);
  minuteStartSpinBox->setStyleSheet(hourStylesheet);

  secondStartSpinBox = new QSpinBox(dateStartWidget);
  secondStartSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  secondStartSpinBox->setRange(0, 59);
  secondStartSpinBox->setStyleSheet(hourStylesheet);

  // add hour and munite
  dateLeftGridLayout->addWidget(hourLabel, 1, 1);
  dateLeftGridLayout->addWidget(hourStartSpinBox, 1, 2);

  dateLeftGridLayout->addWidget(minuteLabel, 1, 3);
  dateLeftGridLayout->addWidget(minuteStartSpinBox, 1, 4);

  dateLeftGridLayout->addWidget(secondLabel, 1, 5);
  dateLeftGridLayout->addWidget(secondStartSpinBox, 1, 6);
}

void P_PBRecord::initDateBotRightWidget() {
  // widget date right
  dateEndWidget = new QWidget(dateAllBotWidget);
  dateEndLayout = new QVBoxLayout();
  dateEndLayout->setSpacing(5);
  dateEndWidget->setFixedSize(mWidthRight / 2, mHeigthDateBot);
  dateEndWidget->setLayout(dateEndLayout);

  dateEndEditLabel = new QLabel(dateEndWidget);
  dateEndEditLabel->setText(tr("Kết thúc :"));
  QFont fontDateEnd = dateEndEditLabel->font();
  fontDateEnd.setPointSize(13);
  fontDateEnd.setBold(true);
  dateEndEditLabel->setFont(fontDateEnd);

  dateEndEdit = new QDateEdit(dateEndWidget);

  QString dateEndStylesheet =
      "QDateEdit { padding-left: 2px; border-width: 3;height : "
      "30px;font-size:13pt;color:#5858DC;background-color:#fff}";
  dateEndEdit->setStyleSheet(dateEndStylesheet);
  dateEndEdit->setFont(Resources::instance().getLargeBoldButtonFont());
  connect(dateEndEdit, &QDateEdit::dateChanged, calendarEnd,
          &QCalendarWidget::setSelectedDate);
  connect(dateEndEdit, &QDateEdit::dateChanged, calendarEnd,
          &QCalendarWidget::setSelectedDate);
  connect(calendarEnd, &QCalendarWidget::selectionChanged, this,
          &P_PBRecord::selectedDateChangedEnd);

  dateEndEdit->setDisplayFormat("dd-MM-yyyy");

  dateEndEdit->setDateRange(calendarEnd->minimumDate(),
                            calendarEnd->maximumDate());
  dateEndEdit->setDate(calendarEnd->maximumDate());
  dateEndEditLabel->setBuddy(dateEndEdit);

  dateRightGridLayout = new QGridLayout;
  dateRightGridLayout->addWidget(dateEndEditLabel, 0, 2, 1, 3);
  dateRightGridLayout->addWidget(dateEndEdit, 0, 4, 1, 3);

  dateEndLayout->addLayout(dateRightGridLayout);

  QLabel *hourLabel = new QLabel(dateEndWidget);
  hourLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  hourLabel->setText("Giờ");
  //  QFont fontHourEnd = hourLabel->font();
  //  fontHourEnd.setPointSize(12);
  //  fontHourEnd.setBold(true);
  //  hourLabel->setFont(fontHourEnd);
  QLabel *minuteLabel = new QLabel(dateEndWidget);
  minuteLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  minuteLabel->setText("Phút");
  //  minuteLabel->setFont(fontHourEnd);

  QLabel *secondLabel = new QLabel(dateEndWidget);
  secondLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  secondLabel->setText("Giây");
  //  secondLabel->setFont(fontHourEnd);

  QString hourStylesheet =
      "QSpinBox { padding-Left: 2px; border-width: 3;height : "
      "30px;font-size:13pt;color:#5858DC;background-color:#fff}";

  hourEndSpinBox = new QSpinBox(dateEndWidget);
  hourEndSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  hourEndSpinBox->setStyleSheet(hourStylesheet);
  hourEndSpinBox->setRange(0, 23);

  minuteEndSpinBox = new QSpinBox(dateEndWidget);
  minuteEndSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  minuteEndSpinBox->setRange(0, 59);
  minuteEndSpinBox->setStyleSheet(hourStylesheet);

  secondEndSpinBox = new QSpinBox(dateEndWidget);
  secondEndSpinBox->setFont(Resources::instance().getLargeBoldButtonFont());
  secondEndSpinBox->setRange(0, 59);
  secondEndSpinBox->setStyleSheet(hourStylesheet);

  // add hour and munite
  dateRightGridLayout->addWidget(hourLabel, 1, 1);
  dateRightGridLayout->addWidget(hourEndSpinBox, 1, 2);

  dateRightGridLayout->addWidget(minuteLabel, 1, 3);
  dateRightGridLayout->addWidget(minuteEndSpinBox, 1, 4);

  dateRightGridLayout->addWidget(secondLabel, 1, 5);
  dateRightGridLayout->addWidget(secondEndSpinBox, 1, 6);
}

void P_PBRecord::initLeftWidget() {
  // init title and calendar right
  leftWidget = new QWidget(mMainTabWidget);

  leftVBoxLayout = new QVBoxLayout();
  leftVBoxLayout->setAlignment(Qt::AlignTop);
  leftVBoxLayout->setSpacing(0);
  leftVBoxLayout->setMargin(0);
  leftWidget->setLayout(leftVBoxLayout);
  leftWidget->setFixedSize(mWidthRight, mHeigthRight);
  mMainTabWidget->addTab(leftWidget, "Bắt đầu");
  // init widget title
  titleStartWidget = new QWidget(leftWidget);
  titleStartWidget->setFixedSize(mWidthRight, mHeigthTopWidget);
  leftVBoxLayout->addWidget(titleStartWidget);

  titleStartLayout = new QVBoxLayout();
  titleStartLayout->setSpacing(0);
  titleStartLayout->setMargin(0);
  titleStartWidget->setLayout(titleStartLayout);

  leftTitleLabel = new QLabel(titleStartWidget);
  leftTitleLabel->setText("Bắt đầu");
  leftTitleLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  QFont fontLabelStart = leftTitleLabel->font();
  fontLabelStart.setPointSize(13);
  fontLabelStart.setBold(true);

  leftTitleLabel->setFont(fontLabelStart);
  leftTitleLabel->setAlignment(Qt::AlignCenter);
  titleStartLayout->addWidget(leftTitleLabel);

  // widget calendar
  calendarStartWidget = new QWidget(leftWidget);
  leftVBoxLayout->addWidget(calendarStartWidget);
  calendarStartWidget->setFixedSize(mWidthRight, mHeightCalendar);
  calendarStartLayout = new QVBoxLayout();
  calendarStartLayout->setSpacing(0);
  calendarStartLayout->setMargin(0);

  calendarStartWidget->setLayout(calendarStartLayout);

  // init calendar end
  calendarStart = new QCalendarWidget(calendarStartWidget);
  calendarStart->setStyleSheet(
      "QCalendarWidget QWidget { alternate-background-color: rgb(128, 128, "
      "128); }");
  calendarStartLayout->addWidget(calendarStart);
  calendarStart->setFont(Resources::instance().getLargeBoldButtonFont());
  QDate datecurrent = QDate::currentDate();
  calendarStart->setSelectedDate(datecurrent);

  calendarStart->setVerticalHeaderFormat(
      QCalendarWidget::VerticalHeaderFormat(QCalendarWidget::NoVerticalHeader));
  long unixtimeToDay = QDateTime::currentDateTime().toTime_t();
  long unixtimeLateHistoryday = unixtimeToDay - 29 * 24 * 60 * 60;
  QDateTime timeStampHistory;
  timeStampHistory.setTime_t(unixtimeLateHistoryday);
  QDate dateLate = timeStampHistory.date();
  calendarStart->setMinimumDate(dateLate);
  calendarStart->setMaximumDate(QDate::currentDate());

  // firstdaychanged
  calendarStart->setFirstDayOfWeek(Qt::DayOfWeek(Qt::Monday));  // thu hai
  // hien thi tuan trong nam

  //    calendarEnd->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(QCalendarWidget::ISOWeekNumbers));
  // format bold
  QTextCharFormat format;
  format.setFontWeight(QFont::Bold);
  calendarStart->setHeaderTextFormat(format);
  calendarStart->setGridVisible(true);
}

void P_PBRecord::selectedDateChangedStart() {
  dateStartEdit->setDate(calendarStart->selectedDate());
}

void P_PBRecord::selectedDateChangedEnd() {
  dateEndEdit->setDate(calendarEnd->selectedDate());
}

void P_PBRecord::totalTimeRecordVideoVOD() {
  TimeStampRecord timeStampRecord;
  timeStampRecord = getTimeStampRecord();
  long totalSecond =
      timeStampRecord.endTimeStampRecord - timeStampRecord.startTimeStampRecord;
  QString totalTime = QString::number(totalSecond);
  totalTimeEdit->setText(totalTime);
}

TimeStampRecord P_PBRecord::getTimeStampRecord() {
  TimeStampRecord timeStampRecord;
  // start
  qDebug() << "dateSelect start" << calendarEnd->selectedDate();
  QDate dateStart = calendarStart->selectedDate();
  QString dateStringStart = dateStart.toString("dd-MM-yyyy");
  QString hourStart = QString::number(hourStartSpinBox->value());
  QString minuteStart = QString::number(minuteStartSpinBox->value());
  QString secondStart = QString::number(secondStartSpinBox->value());
  qDebug() << dateStringStart << hourStart << minuteStart << secondStart;
  long timeStampStart = GeneralDefine::instance().convertDateToTimeStamp(
      dateStringStart, hourStart, minuteStart, secondStart);
  // end
  QDate dateEnd = calendarEnd->selectedDate();
  QString dateStringEnd = dateEnd.toString("dd-MM-yyyy");
  QString hourEnd = QString::number(hourEndSpinBox->value());
  QString minuteEnd = QString::number(minuteEndSpinBox->value());
  QString secondEnd = QString::number(secondEndSpinBox->value());
  qDebug() << dateStringEnd << hourEnd << minuteEnd << secondEnd;
  long timeStampEnd = GeneralDefine::instance().convertDateToTimeStamp(
      dateStringEnd, hourEnd, minuteEnd, secondEnd);

  timeStampRecord.startTimeStampRecord = timeStampStart;
  timeStampRecord.endTimeStampRecord = timeStampEnd;

  qDebug() << "Thoi gian timestamp bat dau" << timeStampStart << "ket thuc"
           << timeStampEnd;
  return timeStampRecord;
}

void P_PBRecord::startRecordDetail() {
  labelMessenger = new QLabel(botControlWidget);
  TimeStampRecord timeStampRecord;
  timeStampRecord = getTimeStampRecord();
  if (timeStampRecord.startTimeStampRecord <
      timeStampRecord.endTimeStampRecord) {
    mButtonStartRecord->setText("Đang tải về");
    botProcesserWidget->show();
    mButtonStartRecord->setEnabled(false);

    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<TimeStampRecord>(timeStampRecord);
    control()->newUserAction(Message.APP_PLAY_BACK_START_RECORD_QUICK,
                             dataStruct);
    //    Q_EMIT StartRecordMore(timeStampRecord);
  } else {
    QMessageBox msgBox;
    msgBox.setText("Thời gian sau phải lớn hơn thời gian trước");
    msgBox.exec();
  }
}

void P_PBRecord::recordDefalt() { /* Q_EMIT StartRecordDefalt(); */
}

void P_PBRecord::closeMenuRecord() {
  updateMaxDayCalendar();
  control()->newUserAction(Message.APP_PLAY_BACK_CLOSE_MENU_RECORD_QUICK,
                           Q_NULLPTR);
  //  Q_EMIT closeMenuRecord();
  ;
}

void P_PBRecord::downloadVideoRecordSuccess() {
  qDebug() << "download success";
  mButtonStartRecord->setText("Chiết xuất");
  mButtonStartRecord->setEnabled(true);
  processBar->setValue(0);  // With a current value of
  botProcesserWidget->hide();
}

void P_PBRecord::updateProcessBar(int percent) {
  QString danger =
      "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, "
      "y2: 0,stop: 0 #F7F772,stop: 0.4999 #FF0020,stop: 0.5 #FF0019,stop: 1 "
      "#FF0000 );border-bottom-right-radius: 5px;border-bottom-left-radius: "
      "5px;border: .px solid black;}";
  QString safe =
      "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, "
      "y2: 0,stop: 0 #78d,stop: 0.4999 #46a,stop: 0.5 #45a,stop: 1 #238 "
      ");border-bottom-right-radius: 7px;border-bottom-left-radius: "
      "7px;border: 1px solid black;}";
  processBar->setValue(percent);
  if (processBar->value() < 80)
    processBar->setStyleSheet(danger);
  else
    processBar->setStyleSheet(safe);
}

void P_PBRecord::linkRecordNotFound() {
  QMessageBox msgBox;
  msgBox.setText("Không thể record được");
  msgBox.exec();
  mButtonStartRecord->setText("Chiết xuất");
  mButtonStartRecord->setEnabled(true);
  processBar->setValue(0);  // With a current value of
  botProcesserWidget->hide();
}

void P_PBRecord::updateMaxDayCalendar() {
  long unixtimeToDay = QDateTime::currentDateTime().toTime_t();
  long unixtimeLateHistoryday = unixtimeToDay - 29 * 24 * 60 * 60;
  QDateTime timeStampHistory;
  timeStampHistory.setTime_t(unixtimeLateHistoryday);
  QDate dateLate = timeStampHistory.date();
  calendarStart->setMinimumDate(dateLate);
  calendarStart->setMaximumDate(QDate::currentDate());
  calendarEnd->setMinimumDate(dateLate);
  calendarEnd->setMaximumDate(QDate::currentDate());

  dateEndEdit->setDateRange(calendarEnd->minimumDate(),
                            calendarEnd->maximumDate());

  dateStartEdit->setDateRange(calendarStart->minimumDate(),
                              calendarStart->maximumDate());
}
