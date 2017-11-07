#include "site.h"

Site::Site(QObject *parent) : QObject(parent) {}

Site *Site::parseFrom(QJsonObject jsonObject) {
  Site *site = new Site(NULL);
  QJsonValue value;
  value = jsonObject.take("site_address");
  if (!value.isNull()) {
    site->setSiteAddress(value.toString());
  }

  value = jsonObject.take("site_id");
  if (!value.isNull()) {
    site->setSiteId(value.toInt());
  }

  value = jsonObject.take("site_image");
  if (!value.isNull()) {
    site->setSiteImage(value.toString());
  }

  value = jsonObject.take("site_name");
  if (!value.isNull()) {
    site->setSiteName(value.toString());
  }
  return site;
}

void Site::log() {
  //    qDebug() << this;
  //    qDebug() << mSiteAddress;
  //    qDebug() << mSiteId;
  //    qDebug() << mSiteImage;
  //    qDebug() << mSiteName;
}

void Site::setSiteAddress(QString siteAddress) {
  this->mSiteAddress = siteAddress;
}

void Site::setSiteId(int siteId) { this->mSiteId = siteId; }

void Site::setSiteImage(QString siteImage) { this->mSiteImage = siteImage; }

void Site::setSiteName(QString siteName) { this->mSiteName = siteName; }

QString Site::getSiteAddress() { return mSiteAddress; }

int Site::getSiteId() { return mSiteId; }

QString Site::getSiteImage() { return mSiteImage; }

QString Site::getSiteName() { return mSiteName; }
