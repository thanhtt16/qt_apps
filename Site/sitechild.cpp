#include "Site/sitechild.h"
#include "Authentication/user.h"
const QString SiteChild::API_CMS_VIEW_SITE = "/cms_api/getSite";



SiteChild::SiteChild(QObject *parent) : QObject(parent) {}
void SiteChild::fetchListSiteChild(int siteId, QString token,
                                   std::function<void(SiteChild *)> onSuccess,
                                   std::function<void(QString)> onFailure) {
    std::function<void(QJsonObject)> onFetchSuccess =
            [onSuccess](QJsonObject jsonObject) {
        SiteChild *site = parseFrom(jsonObject);
        onSuccess(site);
    };
    std::function<void(QString)> onFetchFailure = [onFailure](QString message) {
        onFailure(message);
    };
    QMap<QString, QString> params;
    params["site_id"] = QString::number(siteId);
    params["token"] = token;
    NetworkUtils::instance().getRequest(API_CMS_VIEW_SITE, params, onFetchSuccess,
                                        onFetchFailure);
}
SiteChild *SiteChild::parseFrom(QJsonObject jsonObject) {
    SiteChild *siteList = new SiteChild(NULL);
    QJsonObject jsonObjectSite = jsonObject.take("site").toObject();
    QJsonValue value;
    value = jsonObjectSite.take("site_address");
    if (!value.isNull()) {
        siteList->setSiteAddress(value.toString());
    }

    value = jsonObjectSite.take("site_name");
    if (!value.isNull()) {
        siteList->setSiteName(value.toString());
    }
    value = jsonObjectSite.take("site_id");
    if (!value.isNull()) {
        int siteid = value.toInt();
        siteList->setSiteId(siteid);
    }

    value = jsonObjectSite.take("site_image");
    if (!value.isNull()) {
        siteList->setSiteImage(value.toString());
    }

    Site *workerSite = new Site(Q_NULLPTR);
    workerSite->setSiteName(siteList->getSiteName());
    workerSite->setSiteImage(siteList->getSiteImage());
    workerSite->setSiteId(siteList->getSiteId());
    workerSite->setSiteAddress(siteList->getSiteAddress());

    siteList->appendListSiteChild(workerSite);

    QJsonArray siteChildArray = jsonObjectSite["children"].toArray();
    for (int index = 0; index < siteChildArray.size(); ++index) {
        Site *siteChild = Site::parseFrom(siteChildArray.at(index).toObject());
        siteList->appendListSiteChild(siteChild);
    }
    return siteList;
}

void SiteChild::setSiteAddress(QString siteAddress) {
    this->mSiteAddress = siteAddress;
}

void SiteChild::setSiteId(int siteId) { this->mSiteId = siteId; }

void SiteChild::setSiteImage(QString siteImage) {
    this->mSiteImage = siteImage;
}

void SiteChild::setSiteName(QString siteName) { this->mSiteName = siteName; }

QString SiteChild::getSiteAddress() { return mSiteAddress; }

int SiteChild::getSiteId() { return mSiteId; }

QString SiteChild::getSiteImage() { return mSiteImage; }

QString SiteChild::getSiteName() { return mSiteName; }

QList<Site *> SiteChild::getListSite() { return mListSite; }

void SiteChild::appendListSiteChild(Site *site) { mListSite.append(site); }

void SiteChild::clearListSite(){
    mListSite.clear();
}
