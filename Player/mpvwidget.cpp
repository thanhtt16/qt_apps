#include "mpvwidget.h"
#include <QWheelEvent>
#include <QtCore/QMetaObject>
#include <QtGui/QOpenGLContext>
#include <stdexcept>

static void wakeup(void *ctx) {
  // call slot void onMpvEvents();
  QMetaObject::invokeMethod((MpvWidget *)ctx, "onMpvEvents",
                            Qt::QueuedConnection);
}

static void *get_proc_address(void *ctx, const char *name) {
  Q_UNUSED(ctx);
  QOpenGLContext *glctx = QOpenGLContext::currentContext();
  if (!glctx) {
    return NULL;
  }
  return (void *)glctx->getProcAddress(QByteArray(name));
}

MpvWidget::MpvWidget(QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f) {
  setMouseTracking(true);
  installEventFilter(this);
  mHandle = mpv::qt::Handle::FromRawHandle(mpv_create());
  if (!mHandle) {
    throw std::runtime_error("could not create mpv context");
  }
  //  mpv_set_option_string(mHandle, "terminal", "yes");
  //  mpv_set_option_string(mHandle, "msg-level", "all=all");
  if (mpv_initialize(mHandle) < 0) {
    throw std::runtime_error("could not initialize mpv context");
  }
  //  mpv::qt::set_option_variant(mHandle, "network-timeout", "200");

  // Make use of the MPV_SUB_API_OPENGL_CB API.
  mpv::qt::set_option_variant(mHandle, "vo", "opengl-cb");

  // Request hw decoding, just for testing.
  mpv::qt::set_option_variant(mHandle, "hwdec", /*"auto"*/ "no");
  mpv::qt::set_option_variant(mHandle, "cache", "no");
  mpv::qt::set_option_variant(mHandle, "cache-default", "no");

  pMpvOpenGlContext =
      (mpv_opengl_cb_context *)mpv_get_sub_api(mHandle, MPV_SUB_API_OPENGL_CB);
  if (!pMpvOpenGlContext) {
    throw std::runtime_error("OpenGL not compiled in");
  }
  mpv_opengl_cb_set_update_callback(pMpvOpenGlContext, MpvWidget::onUpdate,
                                    (void *)this);
  connect(this, SIGNAL(frameSwapped()), SLOT(swapped()));

  mpv_observe_property(mHandle, 0, "duration", MPV_FORMAT_DOUBLE);
  mpv_observe_property(mHandle, 0, "time-pos", MPV_FORMAT_DOUBLE);
  mpv_set_wakeup_callback(mHandle, wakeup, this);

  pCheckingTimer = new QTimer(this);
  connect(pCheckingTimer, &QTimer::timeout, this, &MpvWidget::onCheckingState);
}

void MpvWidget::setMainStream(bool isMainStream) {
  mIsMainStream = isMainStream;
}

void MpvWidget::setLiveStream(bool isLiveStream) {
  mIsLiveStream = isLiveStream;
}

void MpvWidget::updateCheckTimeout() {
  mLoadFailureTimeout =
      mIsLiveStream ? ((mIsMainStream ? 4 : 2) * mBaseTimeout) : (mBaseTimeout);
}

void MpvWidget::onCheckingState() {
  if (mCurrentPosition > mLastPosition) {
    if (mPlayerState != Ps_Playing) {
      onPlayerPlaying();
    }
    mLastPosition = mCurrentPosition;
    mUpdateCount = 0;
  } else {
    mUpdateCount++;
    if (mUpdateCount == mLoadFailureTimeout  &&
        !mUrl.isEmpty()) {
      mUrl = "";
      Q_EMIT playerReplay();
      mUpdateCount = 0;
    }
  }
}

double MpvWidget::getCurrentPosition() const { return mCurrentPosition; }

void MpvWidget::setCurrentPosition(double currentPosition) {
  mCurrentPosition = currentPosition;
}

void MpvWidget::startCheckingState() {
  if (!pCheckingTimer->isActive()) {
    pCheckingTimer->start(mCheckingTimer);
    //    qDebug() << Q_FUNC_INFO << mName << mUrl;
  }
}

void MpvWidget::stopCheckingState() {
  if (pCheckingTimer->isActive()) {
    pCheckingTimer->stop();
    //    qDebug() << this << Q_FUNC_INFO << mName << mUrl;
  }
}

void MpvWidget::Command(const QStringList &strlist) {
  int len = strlist.length();
  char **data = new char *[len + 1];
  for (int i = 0; i < len; ++i) {
    data[i] = new char[strlist[i].length() + 1];
    memcpy(data[i], QByteArray(strlist[i].toUtf8()).begin(),
           strlist[i].length() + 1);
  }
  data[len] = NULL;
  AsyncCommand(const_cast<const char **>(data));
  for (int i = 0; i < len; ++i) {
    delete[] data[i];
  }
  delete[] data;
}

void MpvWidget::SetOption(QString key, QString val) {
  QByteArray tmp1 = key.toUtf8(), tmp2 = val.toUtf8();
  HandleErrorCode(
      mpv_set_option_string(mHandle, tmp1.constData(), tmp2.constData()));
}

void MpvWidget::AsyncCommand(const char *args[]) {
  mpv_command_async(mHandle, MPV_REPLY_COMMAND, args);
}

void MpvWidget::Command(const char *args[]) {
  HandleErrorCode(mpv_command(mHandle, args));
}

void MpvWidget::HandleErrorCode(int errorCode) {
  if (errorCode >= 0) {
    return;
  }
  QString error = mpv_error_string(errorCode);
  if (error != QString()) {
    error.append("\n");
  }
}

void MpvWidget::open(QString url, bool isLive, bool isMain) {
    qDebug() << "URL" << url;
  mUpdateCount = 0;
  mCurrentPosition = 0;
  mLastPosition = 0;
  mIsLiveStream = isLive;
  mIsMainStream = isMain;
  updateCheckTimeout();
  if (mUrl != url) {
    mUrl = url;
    if (!mUrl.isEmpty()) {
      mPlayerState = Ps_Stopped;
      const QByteArray tmp = url.toUtf8();
      const char *args[] = {"loadfile", tmp.constData(), NULL};
      Command(args);
      startCheckingState();
      onPlayerLoading();
    } else {
    }
  }
}

void MpvWidget::play() {
    qDebug() << Q_FUNC_INFO ;
  if (mHandle) {
    mPlayPause = 0;
    mpv_set_property_async(mHandle, MPV_REPLY_PROPERTY, "pause",
                           MPV_FORMAT_FLAG, &mPlayPause);
    if (isDebug) {
      qDebug() << this << Q_FUNC_INFO << mUrl;
    }
  }
}

void MpvWidget::pause() {
  if (mHandle) {
    mPlayPause = 1;
    mpv_set_property_async(mHandle, MPV_REPLY_PROPERTY, "pause",
                           MPV_FORMAT_FLAG, &mPlayPause);
    mUpdateCount = 0;
  }
}

void MpvWidget::stop() {
  pause();
  mUrl = "";
  mName = "";
  mIsLiveStream = true;
  mIsMainStream = false;
  stopCheckingState();
  onPlayerStopped();
}

void MpvWidget::seek(int pos, bool relative, bool osd) {
  if (relative) {
    const QByteArray tmp =
        (((pos >= 0) ? "+" : QString()) + QString::number(pos)).toUtf8();
    if (osd) {
      const char *args[] = {"osd-msg", "seek", tmp.constData(), NULL};
      AsyncCommand(args);
    } else {
      const char *args[] = {"seek", tmp.constData(), NULL};
      AsyncCommand(args);
    }
  } else {
    const QByteArray tmp = QString::number(pos).toUtf8();
    if (osd) {
      const char *args[] = {"osd-msg", "seek", tmp.constData(), "absolute",
                            NULL};
      AsyncCommand(args);
    } else {
      const char *args[] = {"seek", tmp.constData(), "absolute", NULL};
      AsyncCommand(args);
    }
  }
}

///
/// \brief MpvWidget::setSpeed
/// \param speed:
/// 0.25: chậm 4x
/// 0.50: chậm 2x
/// 1.00: bình thường
/// 2.00: nhanh 2x
/// 4.00: nhanh 4x
///
void MpvWidget::setSpeed(double speed) {
  mSpeed = speed;
  if (mHandle) {
    mpv_set_property_async(mHandle, MPV_REPLY_PROPERTY, "speed",
                           MPV_FORMAT_DOUBLE, &mSpeed);
    if (isDebug) {
      qDebug() << Q_FUNC_INFO << mSpeed;
    }
  }
}

///
/// \brief MpvWidget::setRenderSize Hàm thay đổi kích thước video thực chất là
/// thay đổi tỷ lệ ngang dọc của video.
/// \param size
///
void MpvWidget::setVideoAspect(QSize size) {
  if (mHandle) {
    QString aspect = QString("%1:%2").arg(size.width()).arg(size.height());
    const QByteArray tmp = aspect.toUtf8();
    const char *args[] = {"set", "video-aspect", tmp.constData(), NULL};
    AsyncCommand(args);
    if (isDebug) {
      qDebug() << Q_FUNC_INFO << aspect;
    }
  }
}
///
/// \brief MpvWidget::zoom Hàm thay đổi tỉ lệ video
/// \param scale Là giá trị trong khoảng từ -20.00 đến 20.00.
/// 0 là tỉ lệ 100%
/// 0.5 là lỉ lệ 150%
/// -0.5 là lỉ lệ 50%
///
void MpvWidget::zoom(double scale) {
  if (scale >= -20 && scale <= 20) {
    mScale = scale;
    const QByteArray tmp = QString::number(mScale).toUtf8();
    const char *args[] = {"set", "video-zoom", tmp.constData(), NULL};
    AsyncCommand(args);
    if (isDebug) {
      qDebug() << Q_FUNC_INFO << mScale << size();
    }
  }
}

void MpvWidget::zoomIn(QPointF center) {
  double scale = mScale + 0.25;
  if (scale <= 5) {
    zoom(scale);
    //    pan(center.x(), center.y());
  }
}

void MpvWidget::zoomOut(QPointF center) {
  double scale = mScale - 0.25;
  if (scale >= 0) {
    zoom(scale);
    if (scale == 0) {
      SetOption("video-pan-x", QString::number(0));
      SetOption("video-pan-y", QString::number(0));
    } else {
      //      pan(center.x(), center.y());
    }
  }
}

///
/// \brief MpvWidget::pan Hàm di chuyển video theo tỉ lệ kích thước video
/// \param x
/// \param y
/// Nếu x = 0.5 y = 0.5 thì tọa độ mới sẽ là 0.5 x width, 0.5 x height
///
void MpvWidget::pan(double x, double y) {
  if (x >= -3 && x <= 3 && y >= -3 && y <= 3) {
    mPanX += x;
    mPanY += y;
    qDebug() << Q_FUNC_INFO << mPanX << mPanY;
    SetOption("video-pan-x", QString::number(mPanX));
    SetOption("video-pan-y", QString::number(mPanY));
  }
}

qreal MpvWidget::getVideoFps() {
  qreal fps = 0.0;
  if (mHandle) {
    mpv_get_property(mHandle, "estimated-vf-fps", MPV_FORMAT_DOUBLE, &fps);
  }
  return fps;
}

qreal MpvWidget::getRenderFps() {
  qreal fps = 0;
  if (mHandle) {
    mpv_get_property(mHandle, "display-fps", MPV_FORMAT_DOUBLE, &fps);
  }
  return fps;
}

QSize MpvWidget::getRenderSize() {
  QSize size;
  int width;
  int height;
  mpv_get_property(mHandle, "width", MPV_FORMAT_INT64, &width);
  mpv_get_property(mHandle, "height", MPV_FORMAT_INT64, &height);
  size.setWidth(width);
  size.setHeight(height);
  return size;
}

QSize MpvWidget::getVideoSize() {
  QSize size;
  int width;
  int height;
  mpv_get_property(mHandle, "dwidth", MPV_FORMAT_INT64, &width);
  mpv_get_property(mHandle, "dheight", MPV_FORMAT_INT64, &height);
  size.setWidth(width);
  size.setHeight(height);
  return size;
}

QString MpvWidget::getVideoCodec() {
  QString codec;
  if (mHandle) {
    codec = mpv_get_property_string(mHandle, "video-codec");
  }
  return codec;
}

double MpvWidget::getVideoAspect() {
  double aspect = 0;
  if (mHandle) {
    mpv_get_property(mHandle, "video-aspect", MPV_FORMAT_DOUBLE, &aspect);
  }
  return aspect;
}

MpvWidget::~MpvWidget() {
  makeCurrent();
  if (pMpvOpenGlContext) {
    mpv_opengl_cb_set_update_callback(pMpvOpenGlContext, NULL, NULL);
  }
  mpv_opengl_cb_uninit_gl(pMpvOpenGlContext);
  if (mHandle) {
    mpv_terminate_destroy(mHandle);
  }
}

void MpvWidget::initializeGL() {
  int r =
      mpv_opengl_cb_init_gl(pMpvOpenGlContext, NULL, get_proc_address, NULL);
  if (r < 0) {
    throw std::runtime_error("could not initialize OpenGL");
  }
}

void MpvWidget::paintGL() {
  mpv_opengl_cb_draw(pMpvOpenGlContext, defaultFramebufferObject(), width(),
                     -height());
}

void MpvWidget::swapped() { mpv_opengl_cb_report_flip(pMpvOpenGlContext, 0); }

void MpvWidget::onMpvEvents() {
  // Process all events, until the event queue is empty.
  while (mHandle) {
    mpv_event *event = mpv_wait_event(mHandle, 0);
    if (event->event_id == MPV_EVENT_NONE) {
      break;
    }
    handleMpvEvent(event);
  }
}

void MpvWidget::handleMpvEvent(mpv_event *event) {
  switch (event->event_id) {
  case MPV_EVENT_PROPERTY_CHANGE: {
    mpvEventPropertyChange(event);
  } break;

  case MPV_EVENT_IDLE: {
  } break;

  case MPV_EVENT_START_FILE: {
  } break;

  case MPV_EVENT_FILE_LOADED: {
  } break;

  case MPV_EVENT_UNPAUSE: {
  } break;

  case MPV_EVENT_PAUSE: {
    onPlayerPaused();
  } break;

  case MPV_EVENT_END_FILE: {
    onPlayerStopped();
  } break;

  case MPV_EVENT_LOG_MESSAGE: {
    mpv_event_log_message *message =
        static_cast<mpv_event_log_message *>(event->data);
    if (message != nullptr) {
      qDebug() << Q_FUNC_INFO << message;
    }
  } break;

  default:
    break;
  }
}

void MpvWidget::onPlayerLoading() {
  mPlayerState = Ps_Loading;
  Q_EMIT playerLoading();
}

void MpvWidget::onPlayerPlaying() {
  mPlayerState = Ps_Playing;
  Q_EMIT playerPlaying();
}

void MpvWidget::onPlayerPaused() {
  mPlayerState = Ps_Paused;
  Q_EMIT playerPaused();
}

void MpvWidget::onPlayerStopped() {
  mPlayerState = Ps_Stopped;
  Q_EMIT playerStopped();
}

void MpvWidget::mpvEventPropertyChange(mpv_event *event) {
  mpv_event_property *prop = (mpv_event_property *)event->data;
  if (strcmp(prop->name, "time-pos") == 0) {
    if (prop->format == MPV_FORMAT_DOUBLE) {
      mCurrentPosition = *(double *)prop->data;
      //      Q_EMIT positionChanged(mCurrentPosition);
      if (isDebug) {
        qDebug() << this << Q_FUNC_INFO << "time-pos" << mIsMainStream << mName
                 << "mCurrentPosition" << mCurrentPosition;
      }
    }
  } else if (strcmp(prop->name, "duration") == 0) {
    if (prop->format == MPV_FORMAT_DOUBLE) {
      double time = *(double *)prop->data;
      Q_EMIT durationChanged(time);
      if (isDebug) {
        qDebug() << this << Q_FUNC_INFO << "duration" << mName << time;
      }
    }
  }
}

// Make Qt invoke mpv_opengl_cb_draw() to draw a new/updated video frame.
void MpvWidget::maybeUpdate() {
  if (window()->isMinimized()) {
    makeCurrent();
    paintGL();
    context()->swapBuffers(context()->surface());
    swapped();
    doneCurrent();
  } else {
    update();
  }
}

void MpvWidget::onUpdate(void *ctx) {
  QMetaObject::invokeMethod((MpvWidget *)ctx, "maybeUpdate");
}
