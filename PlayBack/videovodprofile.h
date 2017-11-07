#ifndef VIDEOVODPROFILE_H
#define VIDEOVODPROFILE_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QString>

class VideoVODProfile : public QObject {
  Q_OBJECT
 private:
  QString src;
  QString resolution;
  QString protocol;
  QString device_id;
  QString network;
  QString mbps;
  QString thumb;

 public:
  VideoVODProfile(QObject *parent);
  static VideoVODProfile *parseVer2Old(QJsonObject jsonObject);
  static VideoVODProfile *parseVer3New(QJsonObject jsonObject);
  QString getSrc();
  QString getResolution();
  QString getProtocol();
  QString getDeviceid();
  QString getNetwork();
  QString getMbps();
  QString getThumb();
};

#endif  // VIDEOVODPROFILE_H
