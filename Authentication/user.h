#ifndef USER_H
#define USER_H
#include <QDataStream>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QTextStream>
#include "Site/site.h"

struct UserInfo {
  QString userData;
};

class User : public QObject {
  Q_OBJECT

 public:
  static QString K_FOLDER;
  static QString K_FILE;
  static QString K_SEPRATOR;

 private:
  int mUserId;
  QString mFullname = "";
  QString mAddress = "";
  QString mEmail = "";
  QString mToken = "";
  Site *mSite = NULL;

 public:
  void log() {
    qDebug() << "User Info";
    qDebug() << "\t Fullname:" << mFullname;
    qDebug() << "\t Address :" << mAddress;
    qDebug() << "\t Email   :" << mEmail;
    qDebug() << "\t Token   :" << mToken;
  }

  User();
  ~User();
  int getUserId();
  void setUserId(int);

  QString getFullName();
  void setFullName(QString);

  QString getToken();
  void setToken(QString);

  QString getEmail();
  void setEmail(QString);

  QString getAddress();
  void setAddress(QString);

  Site *getSite();
  void setSite(Site *);
  static User *parse(QJsonObject json);
  static void saveUserInfo(UserInfo);
  static void clearUserInfo();
  static UserInfo loadUserInfo();
  static User *getUserFrom(UserInfo userInfo);
};

#endif  // USER_H
