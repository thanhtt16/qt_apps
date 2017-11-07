#ifndef MONITORINGCAMERA_H
#define MONITORINGCAMERA_H

#include <QObject>
#include <Camera/caminfo.h>
#include <Common/networkutils.h>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
class MonitoringCamera
{
public:
    QString app_uptime;
    QList<CamInfo*> Cams;
    QString cpu_temp_average;
    QString cpu_used;
    QString ip;
    QString mac;
    QString name;
    QString os_uptime;
    int site_id;
    QString site_name;
    QString total_mem;
    QString used_mem;
    QString os_type;
    float cpu_temp_high;
    float cpu_temp_crit;
    static MonitoringCamera &instance() {
        static MonitoringCamera mInstance;
        return mInstance;
    }
    const static QString API_SYS_CAMERA;
    MonitoringCamera();
    ~MonitoringCamera(){
        int size = Cams.size();
        for (int index = 0; index < size; ++index) {
           CamInfo *info = Cams.last();
           Cams.removeLast();
           delete info;
        }
    }
   void postInfomationCamera(MonitoringCamera* moniterCamera);
private Q_SLOTS:


};

#endif // MONITORINGCAMERA_H
