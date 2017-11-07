#ifndef P_CAM9PLAYEROVERLAY_H
#define P_CAM9PLAYEROVERLAY_H

#include "Camera/camsite.h"
#include "Common/resources.h"
#include "Player/Cam9Player/p_cam9player.h"
#include "c_cam9playeroverlay.h"
#include "Common/generaldefine.h"
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

class C_Cam9PlayerOverlay;

class P_Cam9PlayerOverlay : public Presentation {
private:
  PlayerStyleSheet css;
  PlayerMessage msg;
  bool isFullScreen = false;
  bool isMain = false;

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
  C_Cam9PlayerOverlay *control() { return ((C_Cam9PlayerOverlay *)this->ctrl); }
  P_Cam9PlayerOverlay(Control *ctrl, QWidget *zone);
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

#endif // P_CAM9PLAYEROVERLAY_H
