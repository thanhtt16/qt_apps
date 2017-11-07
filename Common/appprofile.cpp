#include "appprofile.h"

QList<AppProfile *> *AppProfile::appProfiles;

AppProfile::AppProfile(QObject *parent) : QObject(parent) {
  QDateTime createdDate = QDateTime::currentDateTime();
  mCreatedTimestamp = createdDate.toSecsSinceEpoch();
  mModifiedTimestamp = mCreatedTimestamp;
  mId = QString("app_profile_%1").arg(mCreatedTimestamp);
}

AppProfile *AppProfile::cloneAppProfile() {
  AppProfile *appProfile = new AppProfile(Q_NULLPTR);
  appProfile->mName = mName;
  appProfile->mId = mId;
  appProfile->mCreatedTimestamp = mCreatedTimestamp;
  appProfile->mModifiedTimestamp = mModifiedTimestamp;
  appProfile->pAppConfig = pAppConfig->cloneAppConfig();
  return appProfile;
}

int AppProfile::isExist(AppProfile *appProfile) {
  QString id = appProfile->getId();
  for (int index = 0; index < appProfiles->size(); ++index) {
    AppProfile *profile = appProfiles->at(index);
    if (profile->getId() == id) {
      return index;
    }
  }
  return -1;
}

void AppProfile::saveAppProfiles(QList<AppProfile *> *appProfiles) {
  Folders folders;
  Files files;
  QJsonArray jsonArray;
  for (int index = 0; index < appProfiles->size(); ++index) {
    AppProfile *appProfile = appProfiles->at(index);
    jsonArray.append(appProfile->toJsonObject());
  }

  QJsonDocument jsonDocument(jsonArray);
  QString jsonString = QString(jsonDocument.toJson(QJsonDocument::Compact));
  QString rootPath = QDir::homePath();
  rootPath.append(folders.root);
  rootPath.append(folders.configs);
  if (!QDir(rootPath).exists()) {
    QDir().mkpath(rootPath);
  }
  QString filePath = rootPath += files.configs;
  QFile file(filePath);
  file.open(QIODevice::WriteOnly | QIODevice::Truncate);
  QTextStream(&file) << jsonString;
  file.close();
}

QList<AppProfile *> *AppProfile::openAppProfiles() {
  Folders folders;
  Files files;
  if (!appProfiles) {
    appProfiles = new QList<AppProfile *>();
  }
  QString rootPath = QDir::homePath();
  rootPath.append(folders.root);
  rootPath.append(folders.configs);
  if (!QDir(rootPath).exists()) {
    QDir().mkpath(rootPath);
  }
  QString filePath = rootPath += files.configs;
  QFile file(filePath);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly);
    QString data = QTextStream(&file).readAll();
    file.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray jsonArray = jsonDocument.array();
    for (int index = 0; index < jsonArray.size(); ++index) {
      QJsonObject jsonObject = jsonArray.at(index).toObject();
      AppProfile *appProfile = AppProfile::parseFrom(jsonObject);
      appProfiles->append(appProfile);
    }
  } else {
  }
  return appProfiles;
}

QJsonObject AppProfile::toJsonObject() {
  QJsonObject jsonObject;
  jsonObject.insert("name", mName);
  jsonObject.insert("created", mCreatedTimestamp);
  jsonObject.insert("modified", mModifiedTimestamp);
  jsonObject.insert("config", pAppConfig->toJsonObject());
  return jsonObject;
}

AppProfile *AppProfile::parseFrom(QJsonObject jsonObject) {
  AppProfile *appProfile = new AppProfile(NULL);
  appProfile->mName = jsonObject.take("name").toString("");
  appProfile->mCreatedTimestamp =
      jsonObject.take("created").toInt(QDateTime::currentSecsSinceEpoch());
  appProfile->mModifiedTimestamp =
      jsonObject.take("modified").toInt(QDateTime::currentSecsSinceEpoch());
  appProfile->pAppConfig =
      AppConfig::parseFrom(jsonObject.take("config").toObject());
  return appProfile;
}

bool AppProfile::copyConfigFile() {
  Folders folders;
  Files files;
  QString rootPath = QDir::currentPath();
  if (!QDir(rootPath).exists()) {
    QDir().mkpath(rootPath);
  }
  QString filePath = rootPath += files.configs;
  QFile file(filePath);
  if (file.exists()) {
    file.open(QIODevice::ReadOnly);
    QString data = QTextStream(&file).readAll();
    file.close();
    if (data.size() > 0) {
      QString rootPath = QDir::homePath();
      rootPath.append(folders.root);
      rootPath.append(folders.configs);
      if (!QDir(rootPath).exists()) {
        QDir().mkpath(rootPath);
      }
      QString filePath = rootPath += files.configs;
      QFile file(filePath);
      file.open(QIODevice::WriteOnly | QIODevice::Truncate);
      QTextStream(&file) << data;
      file.close();
      return true;
    }
  }
  return false;
}

AppProfile *AppProfile::defaultAppProfile() {
  AppProfile *appProfile = new AppProfile(NULL);
  appProfile->mName = "";
  appProfile->pAppConfig = AppConfig::defaultAppConfig();
  return appProfile;
}

AppProfile *AppProfile::getAppProfile() {
  if (appProfiles && !appProfiles->isEmpty()) {
    return appProfiles->first();
  }
  return Q_NULLPTR;
}

QString AppProfile::getName() { return mName; }

void AppProfile::setName(QString name) { mName = name; }

QString AppProfile::getId() { return mId; }

void AppProfile::setId(QString id) { mId = id; }

int AppProfile::getCreatedTimestamp() { return mCreatedTimestamp; }

void AppProfile::setCreatedTimestamp(int timestamp) {
  mCreatedTimestamp = timestamp;
}

int AppProfile::getModifiedTimestamp() { return mModifiedTimestamp; }

void AppProfile::setModifiedTimestamp(int timestamp) {
  mModifiedTimestamp = timestamp;
}

AppConfig *AppProfile::getAppConfig() { return pAppConfig; }

void AppProfile::setAppConfig(AppConfig *appConfig) { pAppConfig = appConfig; }
