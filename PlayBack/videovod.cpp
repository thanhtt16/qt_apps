#include "PlayBack/videovod.h"
#include "Authentication/user.h"
#include "Common/networkutils.h"
#include "PlayBack/videovodprofile.h"
VideoVOD::VideoVOD(QObject *parent) : QObject(parent) { speedScale = 1.0; }

const QString VideoVOD::API_CMS_VIEW_VOD = "/cms_api/viewVOD";
const QString VideoVOD::API_CMS_VIEW_LIST_VIDEO_VOD =
    "/cms_api/getViewVODMultiCamera";
const QString VideoVOD::API_TOKEN_VIDEO_VOD =
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
    "eyJkZXZpY2VfaWQiOiI4Mjo1ODpmZTozNjo3YjowYSIsImlhdCI6MTQ5NDkyMDEzNywiZXhwIj"
    "oxNTI2NDU2MTM3fQ.ZZ0Eb71lxsfRrDVHA6KHCudguxJFPUP_u2oI-JqEAbc";
// void VideoVOD::fetchOneVideoVOD(QString cameraId,TimeRange
// *timeRangeRequest,std::function<void (VideoVOD *)> onSuccess,
//                             std::function<void (QString)> onFailure){
//    std::function<void(QJsonObject)> onFetchSuccess = [onSuccess](QJsonObject
//    jsonObject){
//        VideoVOD *video  = VideoVOD::parseOneVideo(jsonObject);
//        onSuccess(video);
//    };
//    std::function<void(QString)> onFetchFailure = [onFailure](QString
//    message){
//        onFailure(message);
//    };
//    QMap<QString, QString> params;
//    params["camera_id"] = cameraId;
//    QString valuestarttimeAsString =
//    QString::number(timeRangeRequest->starttime);
//    params["startime"] = valuestarttimeAsString;
//    QString valueStringLength = QString::number(timeRangeRequest->length);
//    params["length"] = valueStringLength;
//    params["token"] = User::instance().getToken();
//    NetworkUtils::instance().getRequest(API_CMS_VIEW_VOD,params,onFetchSuccess,onFetchFailure);
//}

CamItem *VideoVOD::getCamItem() { return camItem; }

void VideoVOD::setCamItem(CamItem *value) { camItem = value; }

double VideoVOD::getSpeedVideo() { return speedVideo; }

void VideoVOD::setSpeedVideo(double value) { speedVideo = value; }

int VideoVOD::getIsShowFullSceen() const { return isShowFullSceen; }

void VideoVOD::setIsShowFullSceen(int value) { isShowFullSceen = value; }

QString VideoVOD::getModeCurrent() const { return modeCurrent; }

void VideoVOD::setModeCurrent(const QString &value) { modeCurrent = value; }

QString VideoVOD::getSdSource() const { return sdSource; }

void VideoVOD::setSdSource(const QString &value) { sdSource = value; }

double VideoVOD::getSpeedScale() const { return speedScale; }

void VideoVOD::setSpeedScale(double value) { speedScale = value; }

QString VideoVOD::getHdSource() const { return hdSource; }

void VideoVOD::setHdSource(const QString &value) { hdSource = value; }

void VideoVOD::fetchOneVideoVOD(QString userToken, QString cameraId,
                                TimeRange timeRangeRequest,
                                std::function<void(VideoVOD *)> onSuccess,
                                std::function<void(QString)> onFailure) {
  std::function<void(QJsonObject)> onFetchSuccess =
      [onSuccess](QJsonObject jsonObject) {
        QJsonArray videoVODJsonArray = jsonObject["data"].toArray();
        QJsonObject jobc = videoVODJsonArray[0].toObject();
        VideoVOD *videoVOD = VideoVOD::parseOneVideo(jobc);
        onSuccess(videoVOD);
      };

  std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };
  QMap<QString, QString> params;
  QJsonObject json;
  params["token"] = userToken;
  json.insert("length", QString::number(timeRangeRequest.length));
  QJsonArray jsarr;
  QJsonObject jsobj;
  jsobj.insert("camera_id", cameraId);
  jsarr.append(jsobj);
  json.insert("list_camera", jsarr);
  json.insert("start_time", QString::number(timeRangeRequest.starttime));
  NetworkUtils::instance().postRequest(API_CMS_VIEW_LIST_VIDEO_VOD, json,
                                       params, onFetchSuccess, onFetchFailure);
}

VideoVOD *VideoVOD::parseOneVideo(QJsonObject jsonObject) {
  QString version =
      AppProfile::getAppProfile()->getAppConfig()->getApiVodVersion();
  VideoVOD *videoVOD = new VideoVOD(NULL);
  QJsonValue jsonValue;
  jsonValue = jsonObject.take("camera_id");
  if (!jsonValue.isNull()) {
    int cameraid = jsonValue.toString().trimmed().toInt();
    videoVOD->cameraId = cameraid;
  }

  QJsonArray videoVODJsonArray = jsonObject["list_src"].toArray();
  if (videoVODJsonArray.size() == 1) {
    for (int i = 0; i < videoVODJsonArray.size(); i++) {
      QJsonObject jsobject = videoVODJsonArray.at(i).toObject();
      VideoVODProfile *videoprofile = VideoVODProfile::parseVer2Old(jsobject);
      videoVOD->videoVODProfile = videoprofile;
    }
  }

  if (videoVODJsonArray.size() == 2) {
    for (int i = 0; i < videoVODJsonArray.size(); i++) {
      QJsonObject videoVODObject = videoVODJsonArray.at(i).toObject();
      VideoVODProfile *videoVODProfile =
          VideoVODProfile::parseVer3New(videoVODObject);
      videoVODProfile->setParent(videoVOD);
      videoVOD->listVideoVODProfile.append(videoVODProfile);
    }
  }
  return videoVOD;
}

int VideoVOD::getCameraId() { return cameraId; }
void VideoVOD::fetchListVideoVOD(
    QString userToken, QList<QString> listCamera, TimeRange timeRangeRequest,
    std::function<void(QList<VideoVOD *>)> onSuccess,
    std::function<void(QString)> onFailure) {
  std::function<void(QJsonObject)> onFetchSuccess =
      [onSuccess](QJsonObject jsonObject) {
        QList<VideoVOD *> listVideo = VideoVOD::parseList(jsonObject);
        onSuccess(listVideo);
      };
  std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };

  QMap<QString, QString> params;
  params["token"] = userToken;
  QJsonObject json;
  json.insert("length", QString::number(timeRangeRequest.length));
  QJsonArray jsarr;
  for (int i = 0; i < listCamera.size(); i++) {
    QJsonObject jsobj;
    jsobj.insert("camera_id", listCamera.at(i));
    jsarr.append(jsobj);
  }

  json.insert("list_camera", jsarr);
  json.insert("start_time", QString::number(timeRangeRequest.starttime));
  NetworkUtils::instance().postRequest(API_CMS_VIEW_LIST_VIDEO_VOD, json,
                                       params, onFetchSuccess, onFetchFailure);
}

QList<VideoVOD *> VideoVOD::parseList(QJsonObject jsonObject) {
  QList<VideoVOD *> listVideoVOD;

  QJsonArray videoVODJsonArray = jsonObject["data"].toArray();

  for (int i = 0; i < videoVODJsonArray.size(); i++) {
    QJsonObject videoVODObject = videoVODJsonArray.at(i).toObject();
    VideoVOD *videoVOD = VideoVOD::parseOneVideo(videoVODObject);
    listVideoVOD.append(videoVOD);
  }

  return listVideoVOD;
}

QList<VideoVODProfile *> VideoVOD::getListProfileVideoVOD() {
  return listVideoVODProfile;
}

QString VideoVOD::getSrcMain() {
  QString urlMain = "";
  if (listVideoVODProfile.size() > 0) {
    for (int i = 0; i < listVideoVODProfile.size(); i++) {
      VideoVODProfile *videoVODProfile = listVideoVODProfile.at(i);
      if (videoVODProfile->getResolution() == "1280x720" &&
          videoVODProfile->getProtocol() == "HLS") {
        urlMain = videoVODProfile->getSrc();
      }
    }
  }
  return urlMain;
}
QString VideoVOD::getSrcSub() {
  QString urlSub = "";
  if (listVideoVODProfile.size() > 0) {
    for (int i = 0; i < listVideoVODProfile.size(); i++) {
      VideoVODProfile *videoVODProfile = listVideoVODProfile.at(i);
      if (videoVODProfile->getResolution() == "720x480" &&
          videoVODProfile->getProtocol() == "HLS") {
        urlSub = videoVODProfile->getSrc();
      }
    }
  }
  return urlSub;
}

VideoVODProfile *VideoVOD::getVideoVODProfile() { return videoVODProfile; }
