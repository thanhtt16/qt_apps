#ifndef APPPROFILE_H
#define APPPROFILE_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QTextStream>
#include "appconfig.h"
#include "resources.h"

struct Files {
  QString  configs = "/configs.json";
};

struct Folders {
  QString root = "/.videowall";
  QString profiles = "/profiles";
  QString configs = "/configs";
};

class AppProfile : public QObject {
  Q_OBJECT
 private:
  Folders folders;
  Files files;
  QString mId;
  QString mName;
  int mCreatedTimestamp;
  int mModifiedTimestamp;
  AppConfig* pAppConfig;

 public:
  explicit AppProfile(QObject* parent = 0);

  static AppProfile* appProfile;
  static QList<AppProfile*>* appProfiles;

  static AppProfile* defaultAppProfile();
  static AppProfile* parseFrom(QJsonObject jsonObject);

  static QList<AppProfile*>* openAppProfiles();
  static void saveAppProfiles(QList<AppProfile*>* appProfiles);
  static int isExist(AppProfile* appProfile);
  static bool copyConfigFile();

  QJsonObject toJsonObject();

  QString getId();
  void setId(QString id);

  QString getName();
  void setName(QString name);

  int getCreatedTimestamp();
  void setCreatedTimestamp(int timestamp);

  int getModifiedTimestamp();
  void setModifiedTimestamp(int timestamp);

  AppConfig* getAppConfig();
  void setAppConfig(AppConfig* appConfig);
  AppProfile* cloneAppProfile();
  static AppProfile* getAppProfile();

 Q_SIGNALS:

 public Q_SLOTS:
};

#endif  // APPPROFILE_H
