#include "videovodprofile.h"

VideoVODProfile::VideoVODProfile(QObject *parent) : QObject(parent) {}

VideoVODProfile *VideoVODProfile::parseVer2Old(QJsonObject jsonObject) {
  VideoVODProfile *videoVODProfile = new VideoVODProfile(NULL);
  QJsonValue jsonValue;
  jsonValue = jsonObject.take("src");
  if (!jsonValue.isNull()) {
    videoVODProfile->src = jsonValue.toString("");
  }

  jsonValue = jsonObject.take("protocol");
  if (!jsonValue.isNull()) {
    videoVODProfile->protocol = jsonValue.toString("");
  }

  jsonValue = jsonObject.take("network");
  if (!jsonValue.isNull()) {
    videoVODProfile->network = jsonValue.toString("");
  }
  return videoVODProfile;
}

VideoVODProfile *VideoVODProfile::parseVer3New(QJsonObject jsonObject) {
  VideoVODProfile *videoVODProfile = new VideoVODProfile(NULL);
  QJsonValue jsonValue;
  jsonValue = jsonObject.take("src");
  if (!jsonValue.isNull()) {
    videoVODProfile->src = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("resolution");
  if (!jsonValue.isNull()) {
    videoVODProfile->resolution = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("protocol");
  if (!jsonValue.isNull()) {
    videoVODProfile->protocol = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("device_id");
  if (!jsonValue.isNull()) {
    videoVODProfile->device_id = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("network");
  if (!jsonValue.isNull()) {
    videoVODProfile->network = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("mbps");
  if (!jsonValue.isNull()) {
    videoVODProfile->mbps = jsonValue.toString("");
  }
  jsonValue = jsonObject.take("thumb");
  if (!jsonValue.isNull()) {
    videoVODProfile->thumb = jsonValue.toString("");
  }
  return videoVODProfile;
}

QString VideoVODProfile::getSrc() { return src; }
QString VideoVODProfile::getResolution() { return resolution; }
QString VideoVODProfile::getProtocol() { return protocol; }
QString VideoVODProfile::getDeviceid() { return device_id; }
QString VideoVODProfile::getNetwork() { return network; }
QString VideoVODProfile::getMbps() { return mbps; }
QString VideoVODProfile::getThumb() { return thumb; }
