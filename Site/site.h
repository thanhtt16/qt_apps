#ifndef SITE_H
#define SITE_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

class Site : public QObject {
  Q_OBJECT
 private:
  QString mSiteAddress;
  int mSiteId;
  QString mSiteImage;
  QString mSiteName;

 public:
  Site(QObject* parent = Q_NULLPTR);

  void setSiteAddress(QString);
  void setSiteId(int);
  void setSiteImage(QString);
  void setSiteName(QString);

  QString getSiteAddress();
  int getSiteId();
  QString getSiteImage();
  QString getSiteName();

  void log();

  static Site* parseFrom(QJsonObject);
 Q_SIGNALS:

 public Q_SLOTS:
};

#endif  // SITE_H
