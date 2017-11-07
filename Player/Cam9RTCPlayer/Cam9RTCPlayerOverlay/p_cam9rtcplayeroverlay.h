#ifndef P_Cam9RTCPlayerOverlay_H
#define P_Cam9RTCPlayerOverlay_H

#include "Camera/camsite.h"
#include "Common/resources.h"
#include "Player/Cam9RTCPlayer/p_cam9rtcplayer.h"
#include "c_cam9rtcplayeroverlay.h"
#include <Common/generaldefine.h>
#include <PacModel/presentation.h>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QMovie>
#include <QObject>
#include <QPoint>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "Common/generaldefine.h"
class C_Cam9RTCPlayerOverlay;

class P_Cam9RTCPlayerOverlay : public Presentation {
private:
  PlayerStyleSheet css;
  PlayerMessage msg;
  bool isFullScreen = false;
  bool isMain = false;
  bool isLive = true;

  QWidget *zone;
  QWidget *topBar = Q_NULLPTR;
  QHBoxLayout *topLayout = Q_NULLPTR;
  QWidget *centerZone = Q_NULLPTR;
  QVBoxLayout *centerLayout = Q_NULLPTR;
  QWidget *bottomBar = Q_NULLPTR;
  QHBoxLayout *bottomLayout = Q_NULLPTR;

  QVBoxLayout *mainLayout = Q_NULLPTR;

  // top
  QLabel *statusLabel = Q_NULLPTR;
  QLabel *titleLabel = Q_NULLPTR;
  QWidget *subTopWidget = Q_NULLPTR;
  QWidget *topRightZone = Q_NULLPTR;
  QPushButton *sdButton = Q_NULLPTR;
  QPushButton *hdButton = Q_NULLPTR;
  QPushButton *hideFullscreenButton = Q_NULLPTR;
  QPushButton *showFullscreenButton = Q_NULLPTR;

  // center
  QLabel *messageLabel = Q_NULLPTR;
  QMovie *loaderMovie = Q_NULLPTR;

  // bottom
  QPushButton *normalRecordButton = Q_NULLPTR;
  QPushButton *quickRecordButton = Q_NULLPTR;
  QPushButton *screenShootButton = Q_NULLPTR;

  QPoint pressPos;
  bool isMoving = false;
  int animationDuration = 200;

public:
  CamItem *camItem;
  C_Cam9RTCPlayerOverlay *control() { return ((C_Cam9RTCPlayerOverlay *)this->ctrl); }
  P_Cam9RTCPlayerOverlay(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update();
  QWidget *getZone(int zoneId = 0);
  void showLoader();
  void hideLoader();
  void setState(PlayerState state);
  void setTitle(QString title);

  void playerLoading();
  void playerPlaying();
  void playerPaused();
  void playerStopped();
  void clearStateLive();

  void updateInfo(CamItem *camItem);

  bool getIsLive() const;
  void setIsLive(bool value);

  void displaySelectedSd();
  void displaySelectedHd();

  // animation record
  QPropertyAnimation *pRecordVideoAnimation;
  QGraphicsOpacityEffect *pFadeEffect;
  void animationNormalRecordButton();
  void startAnimationNormalButton();
  void stopAnimationNormalButton();
  void exitPopOutMode();
  void hideAll();
  void showAll();
  void hideControl();
  void showControl();
public Q_SLOTS:
  void onShowFullscreenClicked();
  void onHideFullscreenClicked();

  void onSdClicked();
  void onHdClicked();

  void onNormalRecordButtonClicked();
  void onQuickRecordButtonClicked();
  void onScreenShootButtonClicked();

  bool eventFilter(QObject *watched, QEvent *event);
};

#endif // P_Cam9RTCPlayerOverlay_H
