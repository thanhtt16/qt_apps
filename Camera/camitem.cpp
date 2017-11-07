#include "camitem.h"

CamItem::CamItem(CamSite *parent) : QObject(parent) {
    this->camSite = parent;
    initCamTypes();
}

QString CamItem::getTypeCamera() const
{
    return typeCamera;
}

int CamItem::getServerId() const
{
    return serverId;
}

QString CamItem::getDeviceServerId() const
{
    return deviceServerId;
}

QString CamItem::getHistoryDay() const
{
    return historyDay;
}


int CamItem::getCameraOrder() const
{
    return cameraOrder;
}

void CamItem::initCamTypes() {
    QStringList tvProtocols = {"HTTP", "HTTP", "HTTP", "HLS", "HLS", "HLS"};
    QStringList tvNetworks = {"LAN", "CDN", "WAN", "CDN", "WAN", "VPN"};
    tvTypes.clear();
    for (int index = 0; index < tvProtocols.size(); ++index) {
        CamItemType type;
        type.protocol = tvProtocols.at(index);
        type.network = tvNetworks.at(index);
        tvTypes.enqueue(type);
    }

//    QStringList rtcProtocols = {"WS", "WS"};
//    QStringList rtcNetworks = {"LAN","CDN"};

    QStringList rtcProtocols = {"WS"};
    QStringList rtcNetworks = {"CDN"};

    rtcTypes.clear();
    for (int index = 0; index < rtcProtocols.size(); ++index) {
        CamItemType type;
        type.protocol = rtcProtocols.at(index);
        type.network = rtcNetworks.at(index);
        rtcTypes.enqueue(type);
    }
}

CamItemType CamItem::getNetworkType() const
{
    return networkType;
}

void CamItem::setNetworkType(const CamItemType &value)
{
    networkType = value;
}

TimeRange CamItem::getTimeRange() const { return timeRange; }

void CamItem::setTimeRange(const TimeRange &value) { timeRange = value; }

int CamItem::getTag() const { return tag; }

void CamItem::setTag(int value) { tag = value; }

void CamItem::setDeviceIdCamera(QString deviceIdCamera) {
    this->deviceIdCamera = deviceIdCamera;
}

void CamItem::setPosition(QString position) { this->postion = position; }

void CamItem::setMinimal(QString minimal) { this->minimal = minimal; }

void CamItem::setCameraId(int cameraId) { this->cameraId = cameraId; }

void CamItem::setEmbeddedAudio(QString embeddedAudio) {
    this->embeddedAudio = embeddedAudio;
}

void CamItem::setOrder(int order) { this->order = order; }

QString CamItem::getDeviceIdCamera() const { return deviceIdCamera; }

QString CamItem::getPostion() const { return postion; }

QString CamItem::getMinimal() const { return minimal; }

int CamItem::getCameraId() const { return cameraId; }

int CamItem::getSiteId() const { return siteId; }

int CamItem::getOrder() const { return order; }

QString CamItem::getEmbeddedAudio() const { return embeddedAudio; }

QList<CamProfile *> CamItem::getSourceProfiles() { return sourceProfiles; }

QList<CamProfile *> CamItem::getProfiles() { return profiles; }

bool CamItem::getIsLive() const { return isLive; }

void CamItem::setIsLive(bool value) { isLive = value; }

bool CamItem::getIsFull() const { return isFull; }

void CamItem::setIsFull(bool value) { isFull = value; }

bool CamItem::getIsMain() const { return isMain; }

void CamItem::setIsMain(bool value) { isMain = value; }

QString CamItem::getCdnType() const { return cdnType; }

void CamItem::setCdnType(const QString &value) { cdnType = value; }

CamItem *CamItem::parse(CamSite *camSite, QJsonObject jsonObject) {
    CamItem *camItem = new CamItem(camSite);
    QJsonValue jsonValue;

    jsonValue = jsonObject.take("cdn_type");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camItem->cdnType = value;
    }

    jsonValue = jsonObject.take("device_id_camera");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camItem->deviceIdCamera = value;
    }

    jsonValue = jsonObject.take("camera_id");
    if (!jsonValue.isNull()) {
        int value = jsonValue.toInt();
        camItem->cameraId = value;
    }

    jsonValue = jsonObject.take("site_id");
    if (!jsonValue.isNull()) {
        int value = jsonValue.toInt();
        camItem->siteId = value;
    }

    jsonValue = jsonObject.take("order");
    if (!jsonValue.isNull()) {
        int value = jsonValue.toInt();
        camItem->order = value;
    }

    jsonValue = jsonObject.take("camera_order");
    if (!jsonValue.isNull()) {
        int value = jsonValue.toInt();
        camItem->cameraOrder = value;
    }

    jsonValue = jsonObject.take("postion");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString().trimmed();
        camItem->postion = value.isNull() ? "" : value.trimmed();
    }

    jsonValue = jsonObject.take("embedded_audio");
    if (!jsonValue.isNull()) {
        QString value = jsonValue.toString();
        camItem->embeddedAudio = value.isNull() ? "" : value;
    }

    jsonValue = jsonObject.take("history_day");
    if(!jsonValue.isNull()){
        QString history  = jsonValue.toString();
        camItem->historyDay = history;

    }

    QJsonArray profileJsonArray = jsonObject["profile"].toArray();
    camItem->profiles.clear();
    for (int index = 0; index < profileJsonArray.size(); index++) {
        QJsonObject profileJsonObject = profileJsonArray[index].toObject();
        CamProfile *camProfile = CamProfile::parse(camItem, profileJsonObject);
        camItem->profiles.append(camProfile);
    }

    QJsonArray sourceProfileJsonArray = jsonObject["source_profile"].toArray();
    camItem->sourceProfiles.clear();
    for (int index = 0; index < sourceProfileJsonArray.size(); ++index) {
        QJsonObject sourceProfileJsonObject =
                sourceProfileJsonArray[index].toObject();
        CamProfile *camProfile =
                CamProfile::parse(camItem, sourceProfileJsonObject);
        camItem->sourceProfiles.append(camProfile);
    }

    return camItem;
}

QList<CamStream *> CamItem::getCamStreams(CamItemType camItemType) {
    QList<CamStream *> results;
    if (profiles.size() > 0) {
        for (int profileIndex = 0; profileIndex < profiles.size(); ++profileIndex) {
            CamProfile *selectedCamProfile = profiles.at(profileIndex);
            if (selectedCamProfile) {
                if (selectedCamProfile &&
                        selectedCamProfile->getName().toLower().contains(
                            camItemType.name.toLower())) {
                    QList<CamStream *> camStreams = selectedCamProfile->getStreams();
                    if (camStreams.size() > 0) {
                        for (int streamIndex = 0; streamIndex < camStreams.size();
                             ++streamIndex) {
                            CamStream *camStream = camStreams.at(streamIndex);
                            if (camStream && camStream->getNetwork() == camItemType.network &&
                                    camStream->getProtocol() == camItemType.protocol) {
                                results.append(camStream);
                            }
                        }
                    }
                }
            }
        }
    }
    return results;
}


CamStream *CamItem::getCamStream(CamItemType camItemType) {
    if (profiles.size() > 0) {
        for (int profileIndex = 0; profileIndex < profiles.size(); ++profileIndex) {
            CamProfile *selectedCamProfile = profiles.at(profileIndex);
            if (selectedCamProfile) {

                if (selectedCamProfile &&
                        selectedCamProfile->getName().toLower().contains(
                            camItemType.name.toLower())) {
                    QList<CamStream *> camStreams = selectedCamProfile->getStreams();
                    if (camStreams.size() > 0) {
                        for (int streamIndex = 0; streamIndex < camStreams.size();
                             ++streamIndex) {
                            CamStream *camStream = camStreams.at(streamIndex);
                            if (camStream && camStream->getNetwork() == camItemType.network &&
                                    camStream->getProtocol() == camItemType.protocol) {
                                return camStream;
                            }
                        }
                    }
                }
            }
        }
    }
    return Q_NULLPTR;
}

void CamItem::getAvailableCamStream(bool isMain,
                                    std::function<void(CamStream *)> onSuccess,
                                    std::function<void()> onFailure) {
    if (!isChecking) {
        isChecking = true;
        initCamTypes();
        //    if (tvTypes.isEmpty() || rtcTypes.isEmpty()) {
        //      initCamTypes();
        //      isChecking = false;
        //      onSuccess(Q_NULLPTR);
        //    }

        CamProfile *camProfile = getCamProfile(isMain);
        if (camProfile) {
            getCamStreamByProfile(camProfile,
                                  [this, onSuccess](CamStream *camStream) {
                isChecking = false;
                onSuccess(camStream);
            },
            [this, onFailure] {
                isChecking = false;
                onFailure();
            });
        } else {
            isChecking = false;
            onFailure();
            qDebug() << this << "getAvailableCamStream"
                     << "can't find stream";
        }
    }
}

void CamItem::getAvailableCamStreams(
        std::function<void(QList<CamStream *>)> onSuccess,
        std::function<void()> onFailure) {  //  qDebug() << Q_FUNC_INFO << this;
    if (!isChecking) {
        isChecking = true;
        camStreams.clear();
        //    qDebug() << this->profiles.size() << this->cameraId; /*<<
        //    this->postion;*/
        if (this->profiles.size() == 2) {
            camStreams.append(Q_NULLPTR);
            camStreams.append(Q_NULLPTR);

            if (this->profiles.first()->getName().contains("HD") &&
                    this->profiles.last()->getName().contains("SD")) {
                this->profiles.swap(0, 1);
            }

            if (tvTypes.isEmpty() || rtcTypes.isEmpty()) {
                initCamTypes();
            }
            CamProfile *camProfile = this->profiles.first();
            //      qDebug() << this->order << "check" << camProfile->getName();
            getCamStreamByProfile(camProfile,
                                  [this, onSuccess](CamStream *camStream) {
                if (tvTypes.isEmpty() || rtcTypes.isEmpty()) {
                    initCamTypes();
                }
                //            qDebug() << this->order << "\t s 0"
                //            << camStream->getSource();
                this->camStreams.replace(0, camStream);
                CamProfile *camProfile = this->profiles.last();
                //            qDebug() << this->order << "check"
                //            << camProfile->getName();
                this->getCamStreamByProfile(
                            camProfile,
                            [this, onSuccess](CamStream *camStream) {
                    //                  qDebug() << this->order
                    //                  << "\t s s 1"
                    //                           <<
                    //                           camStream->getSource();
                    this->camStreams.replace(1, camStream);
                    onSuccess(this->camStreams);
                },
                [this, onSuccess] {
                    //                  qDebug() << this->order
                    //                  << "\t f f 1";
                    onSuccess(this->camStreams);
                });
            },
            [this, onSuccess, camProfile] {
                if (tvTypes.isEmpty() || rtcTypes.isEmpty()) {
                    initCamTypes();
                }
                //            qDebug() << this->order << "\t f 0";
                CamProfile *camProfile = this->profiles.last();
                //            qDebug() << this->order << "check"
                //            << camProfile->getName();
                this->getCamStreamByProfile(
                            camProfile,
                            [this, onSuccess](CamStream *camStream) {
                    qDebug() << this->order << "\t f s 1"
                             << camStream->getSource();
                    this->camStreams.replace(1, camStream);
                    onSuccess(this->camStreams);
                },
                [this, onSuccess] {
                    //                  qDebug() << this->order
                    //                  << "\t f f 1";
                    onSuccess(this->camStreams);
                });
            });
        } else {
            onFailure();
            qDebug() << this << "getAvailableCamStream"
                     << "can't find stream";
        }
    }
}

void CamItem::getCamStreamByProfile(CamProfile *camProfile,
                                    std::function<void(CamStream *)> onSuccess,
                                    std::function<void()> onFailure) {
    getCamStreamByType(camProfile,[this, onSuccess](CamStream *camStream) {
        onSuccess(camStream);
    },
    [this, camProfile, onSuccess, onFailure] {
        getCamStreamByProfile(camProfile, onSuccess, onFailure);
    });
}

void CamItem::getCamStreamByType(CamProfile *camProfile,
                                 std::function<void(CamStream *)> onSuccess,
                                 std::function<void(void)> onFailure) {
    if (!tvTypes.isEmpty() && !rtcTypes.isEmpty()) {
        CamItemType type = isTv() ? tvTypes.dequeue() : rtcTypes.dequeue();
        CamStream *camStream = getCamStreamInCamProfileByType(camProfile, type);
        if (camStream) {
            camStream->checkHasConnection(
                        [this, camStream, onSuccess] {
                if (isTv()) {
                    tvTypes.clear();
                } else {
                    rtcTypes.clear();
                }
                onSuccess(camStream);
            },
            [this, onFailure] { onFailure(); });
        } else {
            onFailure();
        }
    }
}

CamStream *CamItem::getCamStreamInCamProfileByType(CamProfile *camProfile,
                                                   CamItemType type) {
    QList<CamStream *> camStreams = camProfile->getStreams();
    for (int index = 0; index < camStreams.size(); ++index) {
        CamStream *camStream = camStreams.at(index);
        if (camStream->getProtocol() == type.protocol &&
                camStream->getNetwork() == type.network) {
            return camStream;
        }
    }
    return Q_NULLPTR;
}

CamProfile *CamItem::getCamProfile(bool isMain) {
    QString camProfileFilter = isMain ? "HD" : "SD";
    for (int profileIndex = 0; profileIndex < this->profiles.size();
         ++profileIndex) {
        CamProfile *camProfile = this->profiles.at(profileIndex);
        if (camProfile->getName().contains(camProfileFilter)) {
            return camProfile;
            break;
        }
    }
    return Q_NULLPTR;
}

QList<CamProfile *> CamItem::getCamProfiles() {
    QList<CamProfile *> camProfiles;
    for (int profileIndex = 0; profileIndex < this->profiles.size();
         ++profileIndex) {
        CamProfile *camProfile = this->profiles.at(profileIndex);
        if (camProfile->getName().contains("SD")) {
            camProfiles.append(camProfile);
            break;
        }
    }
    for (int profileIndex = 0; profileIndex < this->profiles.size();
         ++profileIndex) {
        CamProfile *camProfile = this->profiles.at(profileIndex);
        if (camProfile->getName().contains("HD")) {
            camProfiles.append(camProfile);
            break;
        }
    }
    return camProfiles;
}
