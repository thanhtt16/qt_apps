#ifndef VIDEOVOD_H
#define VIDEOVOD_H
#include <Camera/camitem.h>
#include <Common/appconfig.h>
#include <Common/appprofile.h>
#include <PlayBack/videovodprofile.h>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QString>
#include <QtCore>
#include "Common/generaldefine.h"
#include "QList"
class TimeRange;
class VideoVOD : public QObject {
  Q_OBJECT

 private:
  int cameraId = -1;
  QList<VideoVODProfile *> listVideoVODProfile;
  VideoVODProfile *videoVODProfile;
  CamItem *camItem = Q_NULLPTR;
  double speedVideo = 1.0;
  double speedScale = 1.0;
  int isShowFullSceen = 0;
  QString modeCurrent = "SD";
  QString sdSource;
  QString hdSource;

 public:
  QList<VideoVOD *> mListVideoVOD;
  const static QString API_TOKEN_VIDEO_VOD;
  const static QString API_CMS_VIEW_LIST_VIDEO_VOD;
  const static QString API_CMS_VIEW_VOD;

  static void fetchOneVideoVOD(QString userToken, QString cameraId,
                               TimeRange timeRangeRequest,
                               std::function<void(VideoVOD *)> onSuccess,
                               std::function<void(QString)> onFailure);
  static VideoVOD *parseOneVideo(QJsonObject jsonObject);
  static void fetchListVideoVOD(
      QString userToken, QList<QString> cameraID, TimeRange timeRangeRequest,
      std::function<void(QList<VideoVOD *>)> onSuccess,
      std::function<void(QString)> onFailure);
  static VideoVOD *parseVideo(QJsonObject jsonObject);
  static QList<VideoVOD *> parseList(QJsonObject jsonObject);
  //    VideoVOD* parseOne(QJsonObject jsonObject);
  QList<VideoVODProfile *> getListProfileVideoVOD();
  int getCameraId();
  VideoVOD(QObject *parent);
  QString getSrcSub();
  QString getSrcMain();
  // 2.0
  VideoVODProfile *getVideoVODProfile();
  CamItem *getCamItem();
  void setCamItem(CamItem *value);
  double getSpeedVideo();
  void setSpeedVideo(double value);
  int getIsShowFullSceen() const;
  void setIsShowFullSceen(int value);
  QString getModeCurrent() const;
  void setModeCurrent(const QString &value);
  QString getSdSource() const;
  void setSdSource(const QString &value);
  double getSpeedScale() const;
  void setSpeedScale(double value);
  QString getHdSource() const;
  void setHdSource(const QString &value);
  ~VideoVOD() {}
};

#endif  // VIDEOVOD_H
