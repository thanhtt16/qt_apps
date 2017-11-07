#include "camprofile.h"

CamProfile::CamProfile(CamItem *parent) : QObject(parent) {
  this->camItem = camItem;
}

CamItem *CamProfile::getCamItem() const { return camItem; }

void CamProfile::setCamItem(CamItem *value) { camItem = value; }

CamProfile *CamProfile::parse(CamItem *camItem, QJsonObject jsonObject) {
  CamProfile *camProfile = new CamProfile(camItem);
  QJsonValue jsonValue;

  jsonValue = jsonObject.take("name");
  if (!jsonValue.isNull()) {
    QString value = jsonValue.toString();
    camProfile->name = value.isNull() ? "" : value;
  }

  jsonValue = jsonObject.take("bitrate");
  if (!jsonValue.isNull()) {
    int value = jsonValue.toInt();
    camProfile->bitrate = value;
  }

  jsonValue = jsonObject.take("resolution");
  if (!jsonValue.isNull()) {
    QString value = jsonValue.toString();
    camProfile->resolution = value.isNull() ? "" : value;
  }

  QJsonArray streamJsonArray = jsonObject["streams"].toArray();
  camProfile->streams.clear();
  for (int index = 0; index < streamJsonArray.size(); index++) {
    QJsonObject streamJsonObject = streamJsonArray[index].toObject();
    CamStream *camStream = CamStream::parse(camProfile, streamJsonObject);
    camProfile->streams.append(camStream);
  }
  return camProfile;
}

QString CamProfile::getName() { return name; }

int CamProfile::getBitrate() { return bitrate; }

QString CamProfile::getResolution() { return resolution; }

QList<CamStream *> CamProfile::getStreams() { return streams; }
