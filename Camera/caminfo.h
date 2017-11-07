#ifndef CAMINFO_H
#define CAMINFO_H

#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>
class CamInfo
{

//    "alias": "Vin Tây Ninh 6",
//       "camera_id": 1995,
//       "device_id": "005a2052895e",
//       "name": "005a2052895e_1995",
//       "site_id": 637,
//       "status": 1
public:
    QString alias;
    int camera_id;
    QString device_id;
    QString name;
    int site_id;
    int status;
    CamInfo();
    QString getAlias();
    int getCameraId();
    QString getName();
    int getSiteId();
    int getStatus();
    void setAlias(QString malias);
    void setCameraid(int cameraId);
    void setName(QString mname);
    void setSiteId(int msite);
    void setStatus(int mstatus);
    QJsonObject toJSonObject();
    ~CamInfo(){
    }
};

#endif // CAMINFO_H
