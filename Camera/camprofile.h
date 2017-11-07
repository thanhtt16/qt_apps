#ifndef CAMPROFILE_H
#define CAMPROFILE_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QString>
#include <QtCore>

#include "camitem.h"
#include "camstream.h"

class CamItem;
class CamStream;

class CamProfile : public QObject {
  Q_OBJECT
 private:
  QString name = "";
  int bitrate = 0;
  QString resolution = "";
  QList<CamStream*> streams;

  CamItem* camItem = Q_NULLPTR;

 public:
  static CamProfile* parse(CamItem* camItem, QJsonObject jsonObject);
  CamProfile(CamItem* parent);
  QString getName();
  int getBitrate();
  QString getResolution();
  QList<CamStream*> getStreams();
  CamItem* getCamItem() const;
  void setCamItem(CamItem* value);
};

#endif  // CAMPROFILE_H
