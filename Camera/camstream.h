#ifndef CAMSTREAM_H
#define CAMSTREAM_H

#include <QEventLoop>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QWebSocket>
#include <functional>
#include "camprofile.h"

class CamProfile;

class CamStream : public QObject {
  Q_OBJECT
 private:
  QString thumb = "";
  QString source = "";
  QString network = "";
  QString protocol = "";
  QString deviceId = "";
  QString sourceDataMap = "";
  int channelId = 0;
  bool playing = false;
  bool playable = true;

  bool isConnected = false;

  CamProfile *camProfile = Q_NULLPTR;
 public:
  CamStream(CamProfile *parent);
  static CamStream *parse(CamProfile *camProfile, QJsonObject jsonObject);
  static QList<CamStream *> *initTestCamStreams();
  QString getThumb();
  QString getSource();
  QString getNetwork();
  QString getProtocol();
  QString getDeviceId();
  QString getSourceDataMap();
  void setSourceDataMap(QString sourceDataMap);
  int getChannelId();
  bool isPlaying();
  void setPlaying(bool);
  bool isPlayable();
  void setPlayable(bool);
  CamProfile *getCamProfile() const;
  void setCamProfile(CamProfile *value);
  void setSource(const QString &value);
  void checkHasConnection(std::function<void(void)> onHasConnection,
                          std::function<void(void)> onHasNoConnection);
  void checkRequestConnection(std::function<void(void)> onHasConnection,
                              std::function<void(void)> onHasNoConnection);
  void checkSocketConnection(std::function<void(void)> onHasConnection,
                             std::function<void(void)> onHasNoConnection);
  void checkSocketPortConnection(std::function<void(void)> onHasConnection,
                                 std::function<void(void)> onHasNoConnection);
};

#endif  // CAMSTREAM_H
