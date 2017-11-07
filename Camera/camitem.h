#ifndef CAMITEM_H
#define CAMITEM_H

#include <QDateTime>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QString>
#include <QTimer>
#include <QtCore>

#include "Common/appprofile.h"
#include "Common/generaldefine.h"
#include "camprofile.h"
#include "camsite.h"
#include "camstream.h"
struct CamItemType {
    QString namtv = "namtv";
    QString namrtc = "namrtc";
    QString name = "SD";
    QString protocol = "WS";
    QString network = "LAN";
};
Q_DECLARE_METATYPE(CamItemType)

class CamProfile;
class CamStream;
class CamSite;

class CamItem : public QObject {
    Q_OBJECT
//    device_id_camera	"d02212d8bc76"
//    postion	"VTS_HNI_Thái Hà_Bảo vệ"
//    camera_id	6273
//    site_id	1412
//    type_camera	"vCAM"
//    server_id	7207
//    device_server_id	"a8:17:02:be:25:6f"
//    history_day	"20"
//    total_view	0
//    order	5
//    deployment_situation	0
//    deployment_status	0
//    is_ptz	0
//    lastupdatetime	"2017-09-11T02:24:43.000Z"
//    camera_order	1
//    cdn_type	"namrtc"
//    source_profile


private:
    QString minimal = "";
    QString deviceIdCamera = "";
    QString postion = "";
    int cameraId = -1;
    int siteId = -1;
    QString typeCamera = "";
    int serverId =-1;
    QString deviceServerId = "";
    QString historyDay = "";
    int order = -1;
    int cameraOrder = -1;
    QString cdnType = "";
    QString embeddedAudio = "";
    QList<CamProfile *> sourceProfiles;
    QList<CamProfile *> profiles;
    bool isLive = true;
    bool isFull = false;
    bool isMain = false;
    bool isChecking = false;

    CamSite *camSite = Q_NULLPTR;
    QQueue<CamItemType> tvTypes;
    QQueue<CamItemType> rtcTypes;
    void initCamTypes();
    QList<CamStream *> camStreams;
    CamStream *sdRtcCamStream = Q_NULLPTR;
    CamStream *hdRtcCamStream = Q_NULLPTR;
    TimeRange timeRange;
    int tag = 0;
    CamItemType networkType;

public:
    static CamItem *parse(CamSite *camSite, QJsonObject jsonObject);
    CamItem(CamSite *parent);
    // settter
    void setDeviceIdCamera(QString);
    void setPosition(QString);
    void setMinimal(QString);
    void setCameraId(int);
    void setSiteId(int);
    void setOrder(int);
    void setEmbeddedAudio(QString);
    // getter
    QString getDeviceIdCamera() const;
    QString getPostion() const;
    QString getMinimal() const;
    int getCameraId() const;
    int getSiteId() const;
    int getOrder() const;
    QString getEmbeddedAudio() const;

    QList<CamProfile *> getSourceProfiles();
    QList<CamProfile *> getProfiles();
    QList<CamStream *> getCamStreams(CamItemType);
    CamStream *getCamStream(CamItemType);
    bool getIsLive() const;
    void setIsLive(bool value);
    bool getIsFull() const;
    void setIsFull(bool value);
    bool getIsMain() const;
    void setIsMain(bool value);
    QString getCdnType() const;
    void setCdnType(const QString &value);

    bool isTv() const { return this->cdnType == "namtv" || this->cdnType == ""; }
    bool isRtc() const { return this->cdnType == "namrtc"; }

    void getAvailableCamStream(bool isMain,
                               std::function<void(CamStream *)> onSuccess,
                               std::function<void(void)> onFailure);
    void getAvailableCamStreams(std::function<void(QList<CamStream *>)> onSuccess,
                                std::function<void(void)> onFailure);
    CamProfile *getCamProfile(bool isMain);
    QList<CamProfile *> getCamProfiles();

    void getCamStreamByProfile(CamProfile *camProfile,
                               std::function<void(CamStream *)> onSuccess,
                               std::function<void(void)> onFailure);

    void getCamStreamByType(CamProfile *camProfile,
                            std::function<void(CamStream *)> onSuccess,
                            std::function<void(void)> onFailure);

    CamStream *getCamStreamInCamProfileByType(CamProfile *camProfile,
                                              CamItemType type);
    int getTag() const;
    void setTag(int value);
    TimeRange getTimeRange() const;
    void setTimeRange(const TimeRange &value);
    ~CamItem() {}
    QString getTypeCamera() const;
    int getServerId() const;
    QString getDeviceServerId() const;
    QString getHistoryDay() const;
    int getCameraOrder() const;
    CamItemType getNetworkType() const;
    void setNetworkType(const CamItemType &value);
};

#endif  // CAMITEM_H
