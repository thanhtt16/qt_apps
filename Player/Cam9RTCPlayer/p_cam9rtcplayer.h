#ifndef P_Cam9RTCPlayer_H
#define P_Cam9RTCPlayer_H

#include "../RTCPlayer/RTCPlayer.h"
#include "../M3U8Playback/PlaybackPlayer.h"
#include "Camera/camsite.h"
#include "Common/appconfig.h"
#include "Common/appprofile.h"
#include "PlayBack/videovod.h"
#include "Player/mpvwidget.h"
#include "c_cam9rtcplayer.h"
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

class C_Cam9RTCPlayer;

class P_Cam9RTCPlayer : public Presentation {
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
  bool isActive = false;
  bool isLive = true;
  bool isExist = true;
  bool isMain = false;
  bool isOld = false;

  QString url = "";

  QStackedLayout *mainLayout = Q_NULLPTR;
  QSize videoSize;

  QWidget *wallZone = Q_NULLPTR;
  QWidget *playerZone = Q_NULLPTR;
  QWidget *blackZone = Q_NULLPTR;
  QWidget *overlayZone = Q_NULLPTR;

  QString convertUrlMainToSub(QString urlMain);

public:
  P_Cam9RTCPlayer(Control *ctrl, QWidget *zone);
  C_Cam9RTCPlayer *control() { return (C_Cam9RTCPlayer *)this->ctrl; }
  void changeControl(Control *ctrl);

  QWidget *getZone(int zoneId = 0);
  void adjustLayout(QSize size);

  void cam9Start(CamItem *);
  void clearSource();
  void cam9Pause();

  void playLive(QString url = "");
  void playLive();

  void playBack();

  void setSize(QSize size);

  CamStream *getCamStream();
  CamProfile *getCamProfile();
  CamItem *getCamItem();

  void wheelEventZoomVideo(QVariant *dataStruct);
  void eventMoveVideo(QVariant *dataStruct);

  void updateSizeWhenExitFullScreenMode();
  void updateSizeWhenEnterFullScreenMode();
  void showLoadingZone();

protected:
  bool eventFilter(QObject *watched, QEvent *event);

Q_SIGNALS:
  void setPlayerDisplayName(QString displayName);

public Q_SLOTS:
  void onShowLoadingSign();
  void onShowPlayerLayer();
  void onShowWallLayer();

};

#endif // P_Cam9RTCPlayer_H
