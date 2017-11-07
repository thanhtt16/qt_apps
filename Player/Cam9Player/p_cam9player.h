#ifndef P_CAM9PLAYER_H
#define P_CAM9PLAYER_H

#include "Camera/camsite.h"
#include "Common/appconfig.h"
#include "Common/appprofile.h"
#include "PlayBack/videovod.h"
#include "Player/mpvwidget.h"
#include "c_cam9player.h"
#include "message.h"
#include <PacModel/presentation.h>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QFont>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include <QMovie>
#include <QNetworkReply>
#include <QObject>
#include <QPoint>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QStackedLayout>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include <functional>

class C_Cam9Player;

class P_Cam9Player : public Presentation {
    Q_OBJECT

private:
    QWidget *zone = Q_NULLPTR;
    CamItem *camItem = Q_NULLPTR;

    double speedAllVideo = 1.0;
    QString modeOneVideoVOD = "SD";
    VideoVOD *videoVod = Q_NULLPTR;

    QString versionCurrent = "";
    int order = 0;
    bool isPause = false;
    bool isLive = true;
    bool isMain = false;


    QString url = "";
    QVBoxLayout *mainLayout = Q_NULLPTR;
    QSize videoSize;
    QWidget *topbarSpaceHolder = Q_NULLPTR;
    MpvWidget *mpvPlayer = Q_NULLPTR;
    QWidget *mpvWidget = Q_NULLPTR;
    QWidget *wallZone = Q_NULLPTR;
    QStackedLayout *mpvLayoutStack = Q_NULLPTR;
    QWidget *blackZone = Q_NULLPTR;
    QWidget *overlayZone = Q_NULLPTR;
    CamItemType networkTypeWorking;
    void initUI();
    QString convertUrlMainToSub(QString urlMain);
    TimeRange timerangeCurrent;
public:
    P_Cam9Player(Control *ctrl, QWidget *zone);
    C_Cam9Player *control() { return (C_Cam9Player *)this->ctrl; }
    void changeControl(Control *ctrl);

    QWidget *getZone(int zoneId = 0);
    void adjustLayout(QSize size);

    void cam9Play();
    void cam9Stop();
    void cam9Pause();
    void playHD();
    void playSD();

    void cam9Play(VideoVOD *videoVod);
    void playOneVideoVod(VideoVOD *videoVod);
    void playVOD(QString url = "", double speed = 1.0);
    void playBack();

    QString liveToVod(QString live, TimeRange timeRange);
    void setSize(QSize size);
    void setSpeed(double speed);

    CamStream *getCamStream();
    CamProfile *getCamProfile();
    CamItem *getCamItem();

    void wheelEventZoomVideo(QVariant *dataStruct);
    void eventMoveVideo(QVariant *dataStruct);

    QString getModeOneVideoVOD() const;
    void setModeOneVideoVOD(const QString &value);
    void updateSizeWhenExitFullScreenMode();
    void updateSizeWhenEnterFullScreenMode();
    void updateTimestampCurrent(TimeRange timerangeCurrent);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
    void setPlayerDisplayName(QString displayName);

public Q_SLOTS:
    void onShowLoadingSign();
    void onShowPlayerLayer();
    void onShowWallLayer();

    //    void onPlayerLoading();
    //    void onPlayerPlaying();
    //    void onPlayerStopped();

//    void onPlayerPaused();
    void onPlayerReplay();

    void playVodSd();
    void playVodHd();

    void takeScreenshoot();

    void seekToNewPostion(int newPosition);
};

#endif // P_CAM9PLAYER_H
