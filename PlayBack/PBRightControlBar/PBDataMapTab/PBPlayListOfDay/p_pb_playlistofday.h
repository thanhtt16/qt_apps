#ifndef P_PBPLAYLISTOFDAY_H
#define P_PBPLAYLISTOFDAY_H

#include <Common/resources.h>
#include <PacModel/presentation.h>
#include <QDate>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QListView>
#include <QObject>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include "Common/generaldefine.h"
#include "Common/resources.h"
#include "PlayBack/PBRightControlBar/PBDataMapTab/PBPlayListOfDay/c_pb_playlistofday.h"
#include "message.h"
class C_PBPlayListOfDay;

class P_PBPlayListOfDay : public Presentation {
  struct PlayListItem {
    long timestamp;
    int length;
    QString icon;
    QString timeLabel;
  };

 private:
  QDateTime currentDate;
  QDateTime selectedDate;
  QTime selectedTime;
  QList<PlayListItem *> playList;
  QList<TimeRange *> tmpListChild;

 public:
  //datamap thao
 void updateStateOfHours(QList<int> mapStateHours);


  QDateTime dateSelect;
  int selectedHour = QDateTime::currentDateTime().time().hour();
  QList<QPushButton *> buttons;
  int max = 100;
  int col = 6;
  int row = 50;
  QGridLayout *layout = Q_NULLPTR;
  QWidget *zone = Q_NULLPTR;
  int selectedTimeHour = 0;
  P_PBPlayListOfDay(Control *ctrl, QWidget *zone);
  QTreeWidget *mTreeVODWidget;
  QListView *mListViewTime;
  C_PBPlayListOfDay *control() { return (C_PBPlayListOfDay *)this->ctrl; }
  void displaySelectedPlayListITem();

  void changeControl(Control *ctrl);
  void update(int message, QVariant *attachment);
  void onPlaylistItemSelected();
  void displaySelectedPage();
  QObject *getZone(int zoneId);

  // tree vod
  QList<TimeRange *> listTimeRanges;  // chứa list timerange để tạotree
  QList<long> listHead;               // chứa starttime mảngheader
  QList<int> listChild;               // chứalength
  QHash<int, QList<TimeRange *>>
      maplistChild;  // map chỉ số của ngày, với list timerange của ngàyđó
  QList<TimeRange *> listTimeRange;
  QList<QTreeWidgetItem *> mParentTreeWidgetItems;
  int selectedParentIndex = -1;
  int selectedChildIndex = -1;
  int indexParentPrevious = -1;
  int indexChildPrevious = -1;
  int indexCalendar = 0;

  InfoVideo *mInforVideoOneVideo;

  void displayPlaylistButton();
  void clearPlaylistButton();
  void displaySelectedPlaylistItem();
  void setSelectedDate(QDateTime newDate);
  void setDefaulDate(QDateTime defaulDate);
  void seekToNewPosition();
  void selectDefaulTimeSlot();
  void updateTimeCurrent();
};

#endif  // P_PBPLAYLISTOFDAY_H
