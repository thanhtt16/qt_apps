#include "p_pb_calendar.h"

/**
     * Generic method to override for updating the presention.
     **/

P_PBCalendar::P_PBCalendar(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
    //    zone->setStyleSheet("background-color: #222");
    // init gui object
    QHBoxLayout *calendarVBoxLayout = new QHBoxLayout();
    calendarVBoxLayout->setMargin(0);
    calendarVBoxLayout->setSpacing(0);
    zone->setLayout(calendarVBoxLayout);

    // init calendar start
    mCalendarVOD = new CalendarCustomize(zone);
    mCalendarVOD->setFixedHeight(300);
    connect(mCalendarVOD, &QCalendarWidget::selectionChanged, this, [this] {
        QDate dateSelected = mCalendarVOD->selectedDate();
        mCalendarVOD->setDateSelected(dateSelected);
        QDateTime dateTime = QDateTime(dateSelected);
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue(dateTime);
        this->control()->newUserAction(
                    Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED, dataStruct);
    });
    updateMaxDayCalendar();
    mCalendarVOD->setStyleSheet(
                "QCalendarWidget QWidget { alternate-background-color: rgb(128, 128, "
                "128); }");
    // firstdaychanged
    mCalendarVOD->setFirstDayOfWeek(Qt::DayOfWeek(Qt::Monday));  // thu hai
    //  calendarVBoxLayout->setSizeConstraint(QLayout::SetFixedSize);
    calendarVBoxLayout->addWidget(mCalendarVOD);
    selectDefaultDate();
    currentDate = QDateTime::currentDateTime();

    //  // Style calendar
    mCalendarVOD->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    QTextCharFormat weekendFormat;
    weekendFormat.setForeground(QBrush(QColor(Qt::white), Qt::SolidPattern));

    mCalendarVOD->setWeekdayTextFormat(Qt::Saturday, weekendFormat);
    mCalendarVOD->setWeekdayTextFormat(Qt::Sunday, weekendFormat);
    QTableView *view = mCalendarVOD->findChild<QTableView*>("qt_calendar_calendarview");
    if (view)
    {
        QPalette pal = view->palette();
        pal.setColor(QPalette::Highlight, QColor("#FF6464"));
        view->setPalette(pal);
    }
}

void P_PBCalendar::updateStateOfDate(QList<QHash<QDate, int> > listStateDates)
{
    this->mCalendarVOD->setListStateOfDates(listStateDates);
    QTableView *view = mCalendarVOD->findChild<QTableView*>("qt_calendar_calendarview");
    if(view){
        view->viewport()->update();
    } else update();
}

void P_PBCalendar::update() {}

QObject *P_PBCalendar::getZone(int zoneId) {
    switch (zoneId) {
    case 1:
        return Q_NULLPTR;
    default:
        return Q_NULLPTR;
    }
}




void P_PBCalendar::updateMaxDayCalendar() {
    if (mCalendarVOD) {
        int maxHistoryDay = 30; //default maxhistoryday
//        int maxHistoryDayOfSite =  control()->appContext->getMaxHistorydayOfSite();
//        if(maxHistoryDayOfSite != 0){
//            qDebug() << "FIND SUCCESS MAX HISTORYDAY :: " << maxHistoryDayOfSite;
//            maxHistoryDay = maxHistoryDayOfSite;
//        }
        mCalendarVOD->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
                                                  QCalendarWidget::NoVerticalHeader));
        QDateTime datetimeCurrent = QDateTime::currentDateTime();
        long timestampOfDayCurrent = datetimeCurrent.time().hour()*3600  +  datetimeCurrent.time().minute()*60 +  datetimeCurrent.time().second();
        long unixtimeToDay = QDateTime::currentDateTime().toTime_t();
        long unixtimeLateHistoryday = (unixtimeToDay + 86400) - (30 * 24 * 3600);
        QDateTime timeStampHistory;
        timeStampHistory.setTime_t(unixtimeLateHistoryday);
        QDate dateLate = timeStampHistory.date();
        mCalendarVOD->setMinimumDate(dateLate);
        mCalendarVOD->setMaximumDate(QDate::currentDate());
        QList<QDate> listDates = caculatorDates(dateLate,QDate::currentDate());
        mCalendarVOD->setListDatesMax(listDates);
    }
}



QList<QDate> P_PBCalendar::caculatorDates(QDate minDate,QDate maxDate){
    QList<QDate> listDates;
    long timestampMin = QDateTime(minDate).toTime_t() - 86400;
    long timestampMax = QDateTime(maxDate).toTime_t();
    while (timestampMax > timestampMin) {
        QDate date = QDateTime::fromSecsSinceEpoch(timestampMax).date();
        listDates.append(date);
        timestampMax-= 86400;
    }
    return listDates;
}

void P_PBCalendar::selectDefaultDate() {
    QDate selectedDateDefault;
    QDate currentDate = QDate::currentDate();
    selectedDateDefault = currentDate;

    //  QDate dateUserSession = QDate::fromString(
    //      control()->appContext->getSessionUser()->getPlayback()->selected_date,
    //      "dd:MM:yyyy");
    //  selectedDateDefault = dateUserSession;

    mCalendarVOD->setSelectedDate(selectedDateDefault);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(currentDate);
    control()->newUserAction(Message.APP_PLAY_BACK_CALENDAR_NEW_DAY_SELECTED,
                             dataStruct);
}

/**
     * Method to create a new presentation exactly equals (field by field) to
  *this presentation.
     * @return the created clone
     **/
//    public Presentation getClone(){
//        try{
//            return (Presentation)clone();
//        } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//        presentation.");}
//        return null;
//    }
