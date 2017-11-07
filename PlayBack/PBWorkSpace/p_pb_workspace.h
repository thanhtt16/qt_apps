#ifndef P_PB_WORKSPACE_H
#define P_PB_WORKSPACE_H

#include <PacModel/presentation.h>
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include <QObject>
#include <QPropertyAnimation>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "PlayBack/Recorder/downloadmanager.h"
#include "PlayBack/Recorder/recordvideovod.h"
#include "PlayBack/videovod.h"
#include "Player/cam9playerpool.h"
#include "c_pb_workspace.h"
#include "DataMap/datamapofcamera.h"
class C_PBWorkSpace;
class P_PBWorkSpace : public Presentation {
    // init ui control
private:
    DataMapOfCamera *dataMapOfCamera;
    QString appName = "Playback";
    int animationDuration = 200;
    CamItemType networkTypeWorking;
public:
    void updateNetworkTypeSelected(CamItemType networkTypeSelected);

    //thaodatamap start
    QDate dateSelected;
    int hourSelected;
    void loadDataMapOfCameraAndUpdateGui(bool isMain, int historyDay, int cameraId);
    void updateStateOfDaysInMonth(bool isMain, QList<QDate> listDates);
    void resetStateOfCalendar();

    void updateStateHours(bool isMain, QList<QString> hours,QDate dateSelected);
    void resetStateHours();
    void updateDateSelectedCurrent(QDate dateSelectedCurrent);
    void updateStateSeekbar(bool isMain,QDate dateSelected, int hourSelectedCurrent);
    void resetStateSeekbar();


    //thaodatamap end



    bool isRecordDefault = false;

    bool firstSetLayoutPageDefault = true;
    int isShowFullScreen = 0;
    bool isSetPageDefault = false;
    // start record
    int checkRecord = 0;
    long mTimeStartRecordVOD = 0;
    long mTimeEndRecordVOD = 0;
    int secondRecord = 0;
    TimeRange timeRangeCurrent;
    // end record
    QString mVersionApi;
    VideoVOD *mVideoVOD = Q_NULLPTR;
    QWidget *recorder = Q_NULLPTR;
    QWidget *seekBar = Q_NULLPTR;
    QWidget *gridPlayback = Q_NULLPTR;
    QList<VideoVOD *> listVideoVOD;

    C_PBWorkSpace *control() { return (C_PBWorkSpace *)this->ctrl; }
    P_PBWorkSpace(Control *ctrl, QWidget *zone);


    ~P_PBWorkSpace(){
        delete mVideoVOD;
    }
    void changeControl(Control *ctrl);
    void show(QWidget *zone);
    void update();

    QWidget *getZone(int zoneId);
    void refreshAppShow();
    void refreshNewLayoutPage();
    void refresh(LayoutStruct layout);
    void updateDefaultLayout(LayoutStruct defaultLayout);
    void seekToNewTimeSlot(long beginOfTimeSlot);

    QWidget *zone = Q_NULLPTR;
    QGridLayout *gridLayout = Q_NULLPTR;
    QList<QWidget *> playerZoneList;
    bool isAnimating = false;
    int selectedIndex = -1;
    long selectedIndexCurrent = -1;
    QSize videoSize;
    LayoutStruct selectedLayout;
    LayoutStruct selectedLayoutLast;
    long beginOfTimeSlot = -1;
    //  QList<C_Cam9Player *> players;
    int numberOfPlayingPlayer = 0;



    void stopPlaying();
    QString getAppName() const;
    void setAppName(const QString &value);
    // thao add
    void fetchURLVideoVOD(QString cameraId, TimeRange *timeRange);

    void fetchListVideoVODRequest(QList<QString> listCamera,
                                  TimeRange timeRangeRequest);
    void fetchOneVideoVOD(QString cameraId, TimeRange timeRangeRequest);
    void seekToNewPostion(long newPostion);
    QList<CamItem *> pageCameras;
    void seekToNewPosition(long newPosition);

    TimeRange getTimeRangeCurrent() const;
    void setTimeRangeCurrent(const TimeRange &value);
    void playListVideoVodWithTimeStamp(TimeRange timeRange);
    void playOneVideoVodWithTimeStamp(TimeRange timeRange);

    void fetchOneVideoVODSwitchMode(long playerId);
    void changeSpeedAllVideo(int message, QVariant *dataStruct);

    // record
    TimeStampRecord mTimeRecord;
    VideoVOD *mVideoVODRecord;
    void showRecordDetail();
    void hideRecordDetail();
    void recordVideoDefault();

    void recordVideoQuick(TimeStampRecord timeRecord);
    void fetchVideoURLVODRecord(QString cameraId, TimeRange timeRangeRequest);
    QString convertPathVODToUrlVideoMP4(QString url, QString fileName);
    void setVideoVODRecord(VideoVOD *videoVOD);
    void fetchPathRecordVODRecord(QString url, QString type,
                                  InfomationRecordVOD *infoRecodeVOD);
    QString mUrlVideoRecord = "";
    void seturlVideoRecord(QString url);
    void downloadVideoRecordSuccess();
    void linkRecordNotFound();
    void startAnimationRecordNormalButton();
    void stopAnimationRecordNormalButton();
    void updateProcessBar(int percent);
    void cancelSaveVideoRecord();
    QString liveToVod(QString live, TimeRange timeRange);
    void seekbarToNewPosition(QVariant *dataStruct);

    void startDownLoadVideoRecord(QString urlFileMp4Record);
    void resizePlayerWhenExitFullScreen();
    void resizePlayerWhenEnterFullScreen();
protected:
public Q_SLOTS:
    void showFullCellAt(int index);
    void hideFullCellAt(int index);
};

#endif  // PRESENTATION_H
