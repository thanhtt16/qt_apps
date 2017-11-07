#include "monitoringcamera.h"

const QString MonitoringCamera::API_SYS_CAMERA =
    "http://sys.vp9.vn:3001/api/stb";
MonitoringCamera::MonitoringCamera() {}

void MonitoringCamera::postInfomationCamera(
    MonitoringCamera* monitoringCamera) {
  std::function<void(QJsonObject)> onFetchSuccess =
      [this, monitoringCamera](QJsonObject jsonObject) {
        Q_UNUSED(jsonObject)
        delete monitoringCamera;
      };
  std::function<void(QString)> onFetchFailure =
      [this, monitoringCamera](QString message) {
        qDebug() << message;
        delete monitoringCamera;
      };

  QJsonObject json;

  json.insert("app_uptime", monitoringCamera->app_uptime);
  QJsonArray jsonAraray;
  for (int i = 0; i < monitoringCamera->Cams.size(); i++) {
    QJsonObject object = monitoringCamera->Cams.at(i)->toJSonObject();
    jsonAraray.append(object);
  }

  json.insert("cams", jsonAraray);
  json.insert("cpu_temp_average", monitoringCamera->cpu_temp_average);
  json.insert("cpu_used", monitoringCamera->cpu_used);
  json.insert("ip", monitoringCamera->ip);
  json.insert("mac", monitoringCamera->mac);
  json.insert("os_uptime", monitoringCamera->os_uptime);
  json.insert("site_id", monitoringCamera->site_id);
  json.insert("total_mem", monitoringCamera->total_mem);
  json.insert("used_mem", monitoringCamera->used_mem);
  json.insert("name", monitoringCamera->name);
  json.insert("os_type", monitoringCamera->os_type);
  json.insert("site_name", monitoringCamera->site_name);
  json.insert("cpu_temp_high", monitoringCamera->cpu_temp_high);
  json.insert("cpu_temp_crit", monitoringCamera->cpu_temp_crit);
  NetworkUtils::instance().postRequestMonitering(
      API_SYS_CAMERA, json, onFetchSuccess, onFetchFailure);
}
