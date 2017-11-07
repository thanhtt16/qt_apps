#ifndef SITECHILD_H
#define SITECHILD_H

#include <Common/networkutils.h>
#include <Site/site.h>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
class SiteChild : public QObject {
  Q_OBJECT
  static const QString API_CMS_VIEW_SITE;

 private:
  QString mSiteAddress;
  int mSiteId;
  QString mSiteImage;
  QString mSiteName;
  QList<Site *> mListSite;

 public:
  SiteChild(QObject *parent = 0);

  void setSiteAddress(QString);
  void setSiteId(int);
  void setSiteImage(QString);
  void setSiteName(QString);

  QString getSiteAddress();
  int getSiteId();
  QString getSiteImage();
  QString getSiteName();
  QList<Site *> getListSite();
  void appendListSiteChild(Site *site);
  static void fetchListSiteChild(int siteId, QString token,
                                 std::function<void(SiteChild *)> onSuccess,
                                 std::function<void(QString)> onFailure);
  static SiteChild *parseFrom(QJsonObject jsonObject);
  void clearListSite();
};

#endif  // SITECHILD_H
