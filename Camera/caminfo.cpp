#include "caminfo.h"

CamInfo::CamInfo() {}

QString CamInfo::getAlias() { return alias; }

int CamInfo::getCameraId() { return camera_id; }

QString CamInfo::getName() { return name; }

int CamInfo::getSiteId() { return site_id; }

int CamInfo::getStatus() { return status; }

QJsonObject CamInfo::toJSonObject() {
  QJsonObject json;
  json.insert("alias", alias);
  json.insert("name", name);
  json.insert("camera_id", camera_id);
  json.insert("device_id", device_id);
  json.insert("site_id", site_id);
  json.insert("status", status);
  return json;
}

void CamInfo::setAlias(QString malias) { alias = malias; }

void CamInfo::setCameraid(int cameraid) { camera_id = cameraid; }

void CamInfo::setName(QString mname) { name = mname; }

void CamInfo::setSiteId(int msiteid) { site_id = msiteid; }

void CamInfo::setStatus(int mstatus) { status = mstatus; }
