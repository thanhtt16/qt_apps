#ifndef RECORDDETAILWIDGET_H
#define RECORDDETAILWIDGET_H

#include <Common/generaldefine.h>
#include <QCalendarWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "Common/resources.h"
class RecordDetailWidget : public QWidget {
  Q_OBJECT
 private:
  QWidget *pPanelWidget;
  QVBoxLayout *pPanelLayout;
  QPushButton *pCloseButton;
  //    void createGeneralOptionsGroupBox();
  //    void createDatesGroupBox();
  //    void createTextFormatsGroupBox();
  //    QComboBox *createColorComboBox();
  QWidget *titleEndWidget;
  QVBoxLayout *titleEndLayout;

  QWidget *dateAllBotWidget;
  QHBoxLayout *dateAllHBoxLayout;
  QGridLayout *dateAllGridLayout;

  QWidget *titleStartWidget;
  QVBoxLayout *titleStartLayout;

  QWidget *calendarEndWidget;
  QVBoxLayout *calendarEndLayout;
  QWidget *calendarStartWidget;
  QVBoxLayout *calendarStartLayout;

  QWidget *dateEndWidget;
  QVBoxLayout *dateEndLayout;
  QWidget *dateStartWidget;
  QVBoxLayout *dateStartLayout;
  // start
  QVBoxLayout *mainVLayout;
  QTabWidget *mMainTabWidget;
  // end
  QWidget *leftWidget;
  QVBoxLayout *leftVBoxLayout;
  QWidget *rightWidget;
  QVBoxLayout *rightVBoxLayout;

  QLabel *rightTitleLabel;
  QLabel *leftTitleLabel;
  QCalendarWidget *calendarStart;
  QCalendarWidget *calendarEnd;

  QComboBox *startHour;
  QComboBox *startMinute;
  QComboBox *endHour;
  QComboBox *endMinute;

  QWidget *botWidget;
  QVBoxLayout *botVBoxLayout;
  QVBoxLayout *botProcesserVBoxLayout;
  QWidget *botProcesserWidget;
  QProgressBar *processBar;
  QWidget *botControlWidget;
  QHBoxLayout *botControlHBoxLayout;

  QPushButton *mRecordDefalt;
  QPushButton *mButtonStartRecord;
  QLabel *totalTimeRecord;
  QLineEdit *totalTimeEdit;

  QSpinBox *hourStartSpinBox;
  QSpinBox *minuteStartSpinBox;
  QSpinBox *secondStartSpinBox;

  QSpinBox *hourEndSpinBox;
  QSpinBox *minuteEndSpinBox;
  QSpinBox *secondEndSpinBox;

  QDateEdit *dateStartEdit;
  QLabel *dateStartEditLabel;
  QGridLayout *dateLeftGridLayout;

  QDateEdit *dateEndEdit;
  QLabel *dateEndEditLabel;
  QGridLayout *dateRightGridLayout;

  long timeStampStart = 0;
  long timeStampEnd = 0;

  QLabel *labelMessenger;

 public:
  RecordDetailWidget(QWidget *parent = 0);
  void initUI();

  void initRightWidget();
  void initLeftWidget();
  void initBotWidget();
  void totalTimeRecordVideoVOD();
  TimeStampRecord getTimeStampRecord();

  void initAllBotDateWidget();
  void initDateBotLeftwidget();
  void initDateBotRightWidget();
  void updateMaxDayCalendar();

 private:
 Q_SIGNALS:
  void StartRecordDefalt();
  void closeMenuRecord();
  void StartRecordMore(TimeStampRecord);
 public Q_SLOTS:
  void selectedDateChangedEnd();
  void selectedDateChangedStart();
  void startRecordDetail();
  void recordDefalt();
  void closeRecord();
  void downloadVideoRecordSuccess();
  void updateProcessBar(int percent);
  void linkRecordNotFound();
  void onChangeTabIndex(int index);
};

#endif  // RECORDDETAILWIDGET_H
