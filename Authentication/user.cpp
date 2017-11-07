#include "user.h"
#include <qdebug.h>

User::User() : QObject(Q_NULLPTR) {}

User::~User() {}

QString User::getToken() { return mToken; }

void User::setToken(QString token) { mToken = token; }

QString User::getAddress() { return mAddress; }

void User::setAddress(QString address) { mAddress = address; }

QString User::getEmail() { return mEmail; }

void User::setEmail(QString email) { mEmail = email; }

int User::getUserId() { return mUserId; }

void User::setUserId(int userId) { mUserId = userId; }

QString User::getFullName() { return mFullname; }

void User::setFullName(QString fullname) { mFullname = fullname; }

User *User::parse(QJsonObject json) {
  User *user = new User();
  QJsonValue jsonValue;
  jsonValue = json.take("user_id");
  if (!jsonValue.isNull()) {
    int value = jsonValue.toInt();
    user->mUserId = value;
  }
  jsonValue = json.take("fullname");
  if (!jsonValue.isNull()) {
    QString value = jsonValue.toString();
    user->mFullname = value;
  }
  jsonValue = json.take("address");
  if (!jsonValue.isNull()) {
    int value = jsonValue.toInt();
    user->mAddress = value;
  }
  jsonValue = json.take("email");
  if (!jsonValue.isNull()) {
    QString value = jsonValue.toString();
    user->mEmail = value;
  }

  jsonValue = json.take("token");
  if (!jsonValue.isNull()) {
    QString value = jsonValue.toString();
    user->mToken = value;
  }

  jsonValue = json.take("site");
  if (!jsonValue.isNull()) {
    QJsonObject siteJsonObject = jsonValue.toObject();
    Site *site = Site::parseFrom(siteJsonObject);
    user->setSite(site);
  }

  return user;
}

Site *User::getSite() { return mSite; }

void User::setSite(Site *site) { mSite = site; }

QString User::K_FOLDER = "/.videowall/users/";
QString User::K_FILE = "user.vp9.vn";
QString User::K_SEPRATOR = "|";

void User::saveUserInfo(UserInfo userInfo) {
  qDebug() << "User -> saveUserInfo";
  // qDebug() << userInfo.userData;
  QString path = QDir::homePath();
  path = path.append(K_FOLDER);
  if (!QDir(path).exists()) {
    QDir().mkpath(path);
  }
  path = path.append(K_FILE);
  QFile file(path);
  file.open(QFile::WriteOnly | QFile::Truncate);
  QTextStream(&file) << userInfo.userData;
  file.close();
}

UserInfo User::loadUserInfo() {
  UserInfo userInfo;
  QString path = QDir::homePath();
  path = path.append(K_FOLDER);
  if (!QDir(path).exists()) {
    QDir().mkpath(path);
  }
  path = path.append(K_FILE);
  QFile file(path);
  if (file.exists()) {
    file.open(QFile::ReadOnly);
    userInfo.userData = QTextStream(&file).readAll();
    file.close();
    // qDebug() << userInfo.userData;
  }
  return userInfo;
}

void User::clearUserInfo() {
  QString path = QDir::homePath();
  path = path.append(K_FOLDER);
  if (!QDir(path).exists()) {
    QDir().mkpath(path);
  }
  path = path.append(K_FILE);
  QFile file(path);
  file.remove();
}

User *User::getUserFrom(UserInfo userInfo) {
  if (!userInfo.userData.isEmpty()) {
    QJsonDocument jsonDocument =
        QJsonDocument::fromJson(userInfo.userData.trimmed().toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    User *user = User::parse(jsonObject);
    return user;
  }
  return Q_NULLPTR;
}
