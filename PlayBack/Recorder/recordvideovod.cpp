#include "recordvideovod.h"
const QString RecordVideoVOD::API_RECORD = "/api.php";
RecordVideoVOD::RecordVideoVOD(QObject *parent) : QObject(parent) {}
void RecordVideoVOD::fetchVideoRecordVODNamTv(
    InfomationRecordVOD *infoRecordVOD,
    std::function<void(RecordVideoVOD *)> onSuccess,
    std::function<void(QString)> onFailure) {
  // neu thanh cong thi vao day de pa
  std::function<void(QJsonObject)> onFetchSuccess =
      [onSuccess](QJsonObject jsonObject) {
        RecordVideoVOD *videoRecordVOD = RecordVideoVOD::parse(jsonObject);
        onSuccess(videoRecordVOD);
      };
  std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };
  QMap<QString, QString> params;
  params["action"] = "convertmp4";
  params["hash"] =
      "085f86ea9475f731580827ac4343b0e10573cfd6b172123a93c9f5d9e8cb2e5a";
  params["secret_id"] = "1";
  QString url = infoRecordVOD->url;
  QString https = url.split("://").first();
  QString urlLast = url.split("://").last();
  QString host = https.append("://").append(urlLast.split("/").first());

  QStringList urlList = url.split("\//");
  QString urlNotHttp = urlList.last();
  QString baseUrlString = urlNotHttp.split("/").first();
  url.replace(baseUrlString, "localhost");

  qDebug() << Q_FUNC_INFO << "URL RECORD" << url;

  params["hlslink"] = url;
  params["namefile"] = infoRecordVOD->filename;
  // request se thuc hien dau tien
  // https://2v1.vcam.viettel.vn/20115/catchup/d02212d8c894_4115/namtv-170901-030005-3595.m3u8
  NetworkUtils::instance().getRequestVOD(host.append(API_RECORD), params,
                                         onFetchSuccess, onFetchFailure);
}

void RecordVideoVOD::fetchVideoRecordVODNamRTC(
    InfomationRecordVOD *infoRecordVOD,
    std::function<void(RecordVideoVOD *)> onSuccess,
    std::function<void(QString)> onFailure) {
  // neu thanh cong thi vao day de pa
  std::function<void(QJsonObject)> onFetchSuccess =
      [onSuccess](QJsonObject jsonObject) {
        RecordVideoVOD *videoRecordVOD = RecordVideoVOD::parse(jsonObject);
        onSuccess(videoRecordVOD);
      };
  std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };
  QMap<QString, QString> params;
  params["action"] = "convertmp4";
  params["hash"] =
      "085f86ea9475f731580827ac4343b0e10573cfd6b172123a93c9f5d9e8cb2e5a";
  params["secret_id"] = "1";
  QString url = infoRecordVOD->url;
  QString host = url.split("/rec/").first();

  qDebug() << Q_FUNC_INFO << "URL" << url << "host" << host;
  QStringList urlList = url.split("\//");
  QString urlNotHttp = urlList.last();
  QString baseUrlString = urlNotHttp.split("/").first();
  url.replace(baseUrlString, "localhost");
  qDebug() << Q_FUNC_INFO << "URL RECORD" << url;

  params["hlslink"] = url;
  params["namefile"] = infoRecordVOD->filename;

  // request se thuc hien dau tien
  NetworkUtils::instance().getRequestVOD(host.append(API_RECORD), params,
                                         onFetchSuccess, onFetchFailure);
}
RecordVideoVOD *RecordVideoVOD::parse(QJsonObject jsonObject) {
  QJsonValue jsonValue;
  RecordVideoVOD *resultReCordVideoVOD = new RecordVideoVOD(NULL);
  jsonValue = jsonObject.take("sizefile");
  if (!jsonValue.isNull()) {
    resultReCordVideoVOD->sizefile = jsonValue.toString();
  }
  jsonValue = jsonObject.take("path");
  if (!jsonValue.isNull()) {
    resultReCordVideoVOD->path = jsonValue.toString();
  }
  jsonValue = jsonObject.take("message");
  if (!jsonValue.isNull()) {
    resultReCordVideoVOD->message = jsonValue.toString();
  }
  jsonValue = jsonObject.take("error");
  if (!jsonValue.isNull()) {
    resultReCordVideoVOD->error = jsonValue.toString();
  }
  return resultReCordVideoVOD;
}
QString RecordVideoVOD::getPath() { return path; }
QString RecordVideoVOD::getSizefile() { return sizefile; }
QString RecordVideoVOD::getMessage() { return message; }
QString RecordVideoVOD::getError() { return error; }
