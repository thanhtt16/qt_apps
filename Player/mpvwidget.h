#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H

#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QtWidgets/QOpenGLWidget>
#include <mpv/client.h>
#include <mpv/opengl_cb.h>
#include <mpv/qthelper.hpp>
#define MPV_REPLY_COMMAND 1
#define MPV_REPLY_PROPERTY 2

enum PlayerState { Ps_Loading, Ps_Playing, Ps_Paused, Ps_Stopped };

class MpvWidget Q_DECL_FINAL : public QOpenGLWidget {
  Q_OBJECT
private:
public:
  bool isDebug = false;

  MpvWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
  ~MpvWidget();
  QSize sizeHint() const { return QSize(1920, 1080); }
  void setVideoAspect(QSize size);
  void open(QString url, bool isLive = true, bool isMain = false);
  void play();
  void pause();
  void stop();
  void setSpeed(double speed);
  void seek(int pos, bool relative = false, bool osd = false);
  double getVideoFps();
  double getRenderFps();

  bool isPause() const { return mPlayPause == 1; }
  double getSpeed() const { return mSpeed; }
  int getOrder() const { return mOrder; }
  void setOrder(int order) { mOrder = order; }
  int getDimens() const { return mDimens; }
  void setDimens(int dimens) {
    mDimens = dimens;
    mLayout = mDimens * mDimens;
  }

  void zoom(double scale = 0);
  void pan(double x = 0, double y = 0);

  void Command(const QStringList &strlist);
  void SetOption(QString key, QString val);
  void AsyncCommand(const char *args[]);
  void Command(const char *args[]);
  void HandleErrorCode(int errorCode);

  PlayerState getPlayerState() const { return mPlayerState; }
  QSize getVideoSize();
  QSize getRenderSize();
  QString getVideoCodec();
  double getVideoAspect();

  void updateCheckTimeout();
  void startCheckingState();
  void stopCheckingState();

  bool isMainStream() const { return mIsMainStream; }
  void setMainStream(bool isMainStream);
  bool isLiveStream() const { return mIsLiveStream; }
  void setLiveStream(bool isLiveStream);
  bool isFullScreen() const { return mIsFullScreen; }
  void setFullScreen(bool isFullScreen) { mIsFullScreen = isFullScreen; }

  void setName(QString name) { mName = name; }

  double getCurrentPosition() const;
  void setCurrentPosition(double currentPosition);
  void zoomIn(QPointF center = QPointF(0, 0));
  void zoomOut(QPointF center = QPointF(0, 0));

Q_SIGNALS:
  void playerEvent(int event);
  void durationChanged(int value);
  void positionChanged(int value);

  void playerLoading();
  void playerPlaying();
  void playerPaused();
  void playerStopped();
  void playerReplay();

protected:
  void initializeGL() Q_DECL_OVERRIDE;
  void paintGL() Q_DECL_OVERRIDE;

private Q_SLOTS:
  void swapped();
  void onMpvEvents();
  void maybeUpdate();

  void mpvEventPropertyChange(mpv_event *event);

  void onPlayerLoading();
  void onPlayerPlaying();
  void onPlayerPaused();
  void onPlayerStopped();

  void onCheckingState();

private:
  QString mName = "";
  bool mPressed = false;
  double mPanX = 0.0;
  double mPanY = 0.0;
  QPoint mPressPos;
  QPoint mMovePos;
  quint64 mBaseTimeout = 5;
  quint64 mLoadFailureTimeout = mBaseTimeout;
  quint64 mReloadTimer = 10;
  quint64 mCheckingTimer = 1 * 1000;
  PlayerState mPlayerState = Ps_Loading;
  int mPlayPause = 0;
  double mSpeed = 1;
  double mScale = 0;
  QString mUrl;
  int mOrder = -1;
  int mDimens = 1;
  int mLayout = 1;
  double mCurrentPosition = 0;
  double mLastPosition = 0;
  bool mIsMainStream = false;
  bool mIsFullScreen = false;
  bool mIsLiveStream = true;
  quint64 mUpdateCount = 0;
  QTimer *pCheckingTimer;

  void handleMpvEvent(mpv_event *event);
  static void onUpdate(void *ctx);

  mpv::qt::Handle mHandle;
  mpv_opengl_cb_context *pMpvOpenGlContext;
};

#endif // PLAYERWINDOW_H
