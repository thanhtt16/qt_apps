#ifndef P_PBRecord_H
#define P_PBRecord_H

#include <Common/generaldefine.h>
#include <PacModel/presentation.h>
#include <QCalendarWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "Common/resources.h"
#include "c_pb_record.h"
class C_PBRecord;
class P_PBRecord : public Presentation {
  // init ui control
 private:
  int mWidthRight = 600;
  int mHeigthRight = 490;

  int mHeigthTopWidget = 50;
  int mHeigthDateBot = 100;
  int mHeightCalendar = 250;
  int mHeightBotWidget = 90;
  QWidget *zone;
  QWidget *pPanelWidget;
  QVBoxLayout *pPanelLayout;
  QPushButton *pCloseButton;
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
  P_PBRecord(Control *ctrl, QWidget *zone);
  C_PBRecord *control() { return (C_PBRecord *)this->ctrl; }

  void changeControl(Control *ctrl);
  void update();

  QWidget *getZone(int zoneId);
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
 Q_SIGNALS:
  void StartRecordDefalt();
  void StartRecordMore(TimeStampRecord *);
 public Q_SLOTS:
  void selectedDateChangedEnd();
  void selectedDateChangedStart();
  void startRecordDetail();
  void recordDefalt();
  void closeMenuRecord();
  void downloadVideoRecordSuccess();
  void updateProcessBar(int percent);
  void linkRecordNotFound();
  void onChangeTabIndex(int index);
};

#endif  // PRESENTATION_H
