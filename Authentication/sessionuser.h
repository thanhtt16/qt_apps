#ifndef SESSIONUSER_H
#define SESSIONUSER_H

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTextStream>
struct PLaybackInFo {
  QString appName = "playback";
  int selected_page = -1;
  int selected_layout = -1;
  QString selected_position;
  QString selected_date;
  int selected_time_slot;
};
struct VideoWallInfo {
  QString appName = "videowall";
  int selected_layout = -1;
  int selected_page = -1;
};
class SessionUser : public QObject {
  Q_OBJECT
 public:
  SessionUser(QObject* parent = Q_NULLPTR);

 private:
  //{
  //  "id_user": 185,
  //  "working_site": 156,
  //  "site_name": "VINCAM",
  //  "apps": [
  //    {
  //      "app_name": "playback",
  //      "selected_page": 4,
  //      "selected_layout": 5,
  //      "selected_date": "20/07/2017",
  //      "selected_time_slot": 12,
  //      "selected_position": 123456789
  //    },
  //    {
  //      "app_name": "videowall",
  //      "selected_page": 4,
  //      "selected_layout": 5
  //    }
  //  ]
  //}

  int idUser = -1;
  int idWorkingSite = -1;
  QString siteName = "";
  QString pathSaveMedia = "";
  int pageTransitionStartMode = -1;
  int pageTransitionDelay = -1;
  PLaybackInFo* playback = Q_NULLPTR;
  VideoWallInfo* videowall = Q_NULLPTR;

 public:
  QJsonObject toJsonObject();
  SessionUser* getSessionUser();
  static SessionUser* parseFrom(QJsonObject jsonObject);
  static SessionUser* createDefaultSessionUser();

  int getIdUser() const;
  void setIdUser(int value);
  int getIdWorkingSite() const;
  void setIdWorkingSite(int value);
  QString getSiteName() const;
  void setSiteName(const QString& value);
  PLaybackInFo* getPlayback();
  void setPlayback(PLaybackInFo* value);
  VideoWallInfo* getVideowall();
  void setVideowall(VideoWallInfo* value);
  QString getPathSaveMedia() const;
  void setPathSaveMedia(const QString& value);
  int getPageTransitionStartMode() const;
  void setPageTransitionStartMode(int value);
  int getPageTransitionDelay() const;
  void setPageTransitionDelay(int value);
};

#endif  // SESSIONUSER_H
