#include "camsite.h"

CamSite::CamSite(QObject *parent) : QObject(parent) {}

void CamSite::setLayout(int layout) { this->layout = layout; }

void CamSite::setPlayPage(int page) { this->playPage = page; }

int CamSite::getSumChannel() { return sumChannel; }

void CamSite::setSumChannel(int sumChannel) { this->sumChannel = sumChannel; }

int CamSite::getLayout() { return layout; }

int CamSite::getPlayPage() { return playPage; }

int CamSite::getPlayCamFullScreen() { return playCamFullscreen; }

QList<CamItem *> CamSite::getCamItems() { return camItems; }

void CamSite::setCamItems(QList<CamItem *> newCamItems) {
    camItems = newCamItems;
}

CamSite *CamSite::parseCamItems(QJsonObject jsonObject) {
    CamSite *camSite = new CamSite(NULL);
    QJsonValue jsonValue;

    jsonValue = jsonObject.take("sumchannel");
    if (!jsonValue.isNull()) {
        camSite->sumChannel = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("layout");
    if (!jsonValue.isNull()) {
        camSite->layout = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("play_page");
    if (!jsonValue.isNull()) {
        camSite->playPage = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("play_cam_fullscreen");
    if (!jsonValue.isNull()) {
        camSite->playCamFullscreen = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("TOTAL");
    if (!jsonValue.isNull()) {
        camSite->totalCamItem = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("PER_PAGE");
    if (!jsonValue.isNull()) {
        camSite->perpageCamItem = jsonValue.toInt();
    }

    QJsonArray camJsonArray =
            jsonObject["CAMS_LIST"].toArray();

    camSite->camItems.clear();

    for (int index = 0; index < camJsonArray.size(); index++) {
        QJsonObject camJsonObject = camJsonArray[index].toObject();
        CamItem *camItem = CamItem::parse(camSite, camJsonObject);
        if(camItem != Q_NULLPTR) {
            camSite->camItems.append(camItem);
        }else{
            camSite->camItems.append(Q_NULLPTR);
        }
    }


    for (int index = 0; index < camSite->camItems.size(); ++index) {
        CamItem *camItem = camSite->camItems.at(index);
        if (camItem != Q_NULLPTR) {
            qDebug() << "Cam Item" << index << camItem->getCameraOrder()
                     << camItem->getCameraId() << camItem->getPostion();
        }
    }

    return camSite;
}

CamSite *CamSite::parseWithOrder(bool withOrder, QJsonObject jsonObject) {
    CamSite *camSite = new CamSite(NULL);
    QJsonValue jsonValue;

    jsonValue = jsonObject.take("sumchannel");
    if (!jsonValue.isNull()) {
        camSite->sumChannel = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("layout");
    if (!jsonValue.isNull()) {
        camSite->layout = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("play_page");
    if (!jsonValue.isNull()) {
        camSite->playPage = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("play_cam_fullscreen");
    if (!jsonValue.isNull()) {
        camSite->playCamFullscreen = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("TOTAL");
    if (!jsonValue.isNull()) {
        camSite->totalCamItem = jsonValue.toInt();
    }

    jsonValue = jsonObject.take("PER_PAGE");
    if (!jsonValue.isNull()) {
        camSite->perpageCamItem = jsonValue.toInt();
    }

    QJsonArray camJsonArray =
            jsonObject[withOrder ? "cams_list" : "CAMS_LIST"].toArray();
    camSite->camItems.clear();

    if (!withOrder) {
        camSite->setSumChannel(camJsonArray.size());
        int maxLayout = 25;
        if (maxLayout < 16) {
            camSite->setLayout(maxLayout);
        } else {
            camSite->setLayout(16);
        }
        camSite->setPlayPage(1);
    }

    for (int index = 0; index < camSite->getSumChannel(); ++index) {
        camSite->camItems.append(NULL);
    }
    for (int index = 0; index < camSite->camItems.size(); index++) {
        QJsonObject camJsonObject = camJsonArray[index].toObject();
        CamItem *camItem = CamItem::parse(camSite, camJsonObject);
        if (!withOrder) {
            camItem->setOrder(index);
        }
        camSite->camItems.replace(withOrder ? camItem->getOrder() : index, camItem);
    }
    int total = camSite->camItems.size();
    if (!withOrder) {
        for (int i = 0; i < total - 1; ++i) {
            for (int j = i + 1; j < total; ++j) {
                CamItem *item0 = camSite->camItems.at(i);
                CamItem *item1 = camSite->camItems.at(j);
                if (QString::compare(item0->getPostion(), item1->getPostion(),
                                     Qt::CaseInsensitive) > 0) {
                    camSite->camItems.swap(i, j);
                    item0->setOrder(j);
                    item1->setOrder(i);
                }
            }
        }
    }
    for (int index = 0; index < camSite->camItems.size(); ++index) {
        CamItem *camItem = camSite->camItems.at(index);
        if (camItem != Q_NULLPTR) {
            //      qDebug() << "Cam Item" << index << camItem->getOrder()
            //               << camItem->getCameraId() << camItem->getPostion();
        }
    }

    return camSite;
}

void CamSite::fetch(std::function<void(CamSite *)> onSuccess,
                    std::function<void(QString)> onFailure) {
    std::function<void(QJsonObject)> onFetchSuccess =
            [onSuccess](QJsonObject jsonObject) {
        //        qDebug() << Q_FUNC_INFO << jsonObject;
        CamSite *camSite = CamSite::parseWithOrder(true, jsonObject);
        onSuccess(camSite);
    };
    std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
        onFailure(message);
    };
    QMap<QString, QString> params;
    params["device_id"] = NetworkUtils::instance().getMacAddress();
    params["token"] = NetworkUtils::instance().getToken();
    //  qDebug() << Q_FUNC_INFO << params;
    NetworkUtils::instance().getRequest(
                AppProfile::getAppProfile()->getAppConfig()->getCamSiteByDeviceApiUri(),
                params, onFetchSuccess, onFetchFailure);
}

int CamSite::getTotalCamItem() const { return totalCamItem; }

void CamSite::setTotalCamItem(int value) { totalCamItem = value; }

void CamSite::getCamerasOfSite(int siteId, QString token,
                               std::function<void(CamSite *)> onSuccess,
                               std::function<void(QString)> onFailure) {
    std::function<void(QJsonObject)> onFetchSuccess =
            [onSuccess](QJsonObject jsonObject) {
        CamSite *camSite = CamSite::parseCamItems(jsonObject);
        onSuccess(camSite);
    };
    std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
        onFailure(message);
    };
    QMap<QString, QString> params;
    params["site_id"] = QString::number(siteId);
    params["version"] = "003";
    params["token"] = token;
    NetworkUtils::instance().getRequest("/cms_api/getCamera", params,
                                        onFetchSuccess, onFetchFailure);
}

void CamSite::getCamerasOfSiteWithLayoutPage(int siteId, QString token, int page,
                                             int layout,
                                             std::function<void(CamSite *)> onSuccess,
                                             std::function<void(QString)> onFailure) {
    std::function<void(QJsonObject)> onFetchSuccess =
            [onSuccess](QJsonObject jsonObject) {
        CamSite *camSite = CamSite::parseCamItems(jsonObject);
        onSuccess(camSite);
    };

    std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
        onFailure(message);
    };

    QMap<QString, QString> params;
    params["site_id"] = QString::number(siteId);
    params["p"] = QString::number(page);
    params["c"] = QString::number(layout);
    params["version"] = "004";
    params["token"] = token;
    NetworkUtils::instance().getRequest("/cms_api/getCamera", params,
                                        onFetchSuccess, onFetchFailure);
}

QString CamSite::toString() {
    QString str = "";
    return str;
}

QList<CamItem *> CamSite::getPageCameras(LayoutStruct layout) {
    QList<CamItem *> cameras;
    int beginPage = (layout.selectedPage - 1) * layout.numberOfCameras;
    if (beginPage >= 0) {
        for (int index = beginPage; (index < beginPage + layout.numberOfCameras) &&
             (index < camItems.size());
             ++index) {
            cameras.append(camItems.at(index));
        }
    }
    return cameras;
}


