#ifndef RECORDVIDEOVOD_H
#define RECORDVIDEOVOD_H

#include <QJsonObject>
#include <QObject>
#include <QString>
#include "Common/generaldefine.h"
#include "Common/networkutils.h"
class InfomationRecordVOD;
class RecordVideoVOD : public QObject {
  Q_OBJECT
  const static QString API_RECORD;

 private:
  QString path;
  QString sizefile;
  QString message;
  QString error;

 public:
  static void fetchVideoRecordVODNamTv(
      InfomationRecordVOD *infoRecordVOD,
      std::function<void(RecordVideoVOD *)> onSuccess,
      std::function<void(QString)> onFailure);
  static void fetchVideoRecordVODNamRTC(
      InfomationRecordVOD *infoRecordVOD,
      std::function<void(RecordVideoVOD *)> onSuccess,
      std::function<void(QString)> onFailure);

  static RecordVideoVOD *parse(QJsonObject jsonObject);
  RecordVideoVOD(QObject *parent);
  QString getPath();
  QString getSizefile();
  QString getMessage();
  QString getError();
  ~RecordVideoVOD() {}
};

#endif  // RECORDVIDEOVOD_H
