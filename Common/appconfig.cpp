#include "appconfig.h"

QString AppConfig::getSocketUrl() const { return mSocketUrl; }

void AppConfig::setSocketUrl(const QString &socketUrl) {
    mSocketUrl = socketUrl;
}

int AppConfig::getNumberOfThread() const { return numberOfThread; }

void AppConfig::setNumberOfThread(int value) { numberOfThread = value; }

QString AppConfig::getAppVersion() const
{
    return mAppVersion;
}

void AppConfig::setAppVersion(const QString &appVersion)
{
    mAppVersion = appVersion;
}

QString AppConfig::getDateBuild() const
{
    return mDateBuild;
}

void AppConfig::setDateBuild(const QString &dateBuild)
{
    mDateBuild = dateBuild;
}

AppConfig::AppConfig(QObject *parent) : QObject(parent) {}

AppConfig *AppConfig::cloneAppConfig() {
    AppConfig *appConfig = new AppConfig(Q_NULLPTR);
    appConfig->mPingHost = mPingHost;
    appConfig->mSocketUrl = mSocketUrl;
    appConfig->mBaseApiUrl = mBaseApiUrl;
    appConfig->mLoginApiUri = mLoginApiUri;
    appConfig->mTokenApiUri = mTokenApiUri;
    appConfig->mCamSiteByDeviceApiUri = mCamSiteByDeviceApiUri;
    appConfig->mCamSiteBySiteApiUri = mCamSiteBySiteApiUri;
    appConfig->mBaseRecordApiUrl = mBaseRecordApiUrl;
    appConfig->mRecordApiUri = mRecordApiUri;
    appConfig->mBaseVodApiUrl = mBaseVodApiUrl;
    appConfig->mChangePasswordApiUri = mChangePasswordApiUri;
    appConfig->mBrandName = mBrandName;
    appConfig->mApiVodVersion = mApiVodVersion;
    appConfig->numberOfThread = numberOfThread;
    appConfig->mAppVersion = mAppVersion;
    appConfig->mDateBuild = mDateBuild;
    return appConfig;
}

AppConfig *AppConfig::defaultAppConfig() {
    AppConfig *appConfig = new AppConfig(Q_NULLPTR);
    appConfig->mPingHost = "http://www.google.com";
    appConfig->mSocketUrl = "https://api.vincam.net";
    appConfig->mBaseApiUrl = "https://api.vincam.net";
    appConfig->mLoginApiUri = "/login";
    appConfig->mTokenApiUri = "/getToken";
    appConfig->mCamSiteByDeviceApiUri = "/cms_api/getAllCamOfSiteV2";
    appConfig->mCamSiteBySiteApiUri = "/cms_api/getCamera";
    appConfig->mBaseRecordApiUrl = "https://1vg1.cdn.vincam.net";
    appConfig->mRecordApiUri = "/api.php";
    appConfig->mBaseVodApiUrl = "http://4co2.vp9.tv";
    appConfig->mChangePasswordApiUri = "/cms_api/updateUserPassword";
    appConfig->mBrandName = "VCAM";
    appConfig->mApiVodVersion = "3.0";
    appConfig->mAppVersion = "3.10.5";
    appConfig->mDateBuild = "10/09/2017";
    appConfig->numberOfThread = 10;
    return appConfig;
}

AppConfig *AppConfig::parseFrom(QJsonObject jsonObject) {
    AppConfig *appConfig = new AppConfig(NULL);
    appConfig->mPingHost = jsonObject.take("ping_host").toString("");
    appConfig->mSocketUrl = jsonObject.take("socket_url").toString("");
    appConfig->mBaseApiUrl = jsonObject.take("base_api_url").toString("");
    appConfig->mLoginApiUri = jsonObject.take("login_api_uri").toString("");
    appConfig->mTokenApiUri = jsonObject.take("token_api_uri").toString("");
    appConfig->mCamSiteByDeviceApiUri =
            jsonObject.take("cam_site_by_device_api_uri").toString("");
    appConfig->mCamSiteBySiteApiUri =
            jsonObject.take("cam_site_by_site_api_uri").toString("");
    appConfig->mBaseRecordApiUrl =
            jsonObject.take("base_record_api_url").toString("");
    appConfig->mRecordApiUri = jsonObject.take("record_api_uri").toString("");
    appConfig->mBaseVodApiUrl = jsonObject.take("base_vod_api_url").toString("");
    appConfig->mBrandName = jsonObject.take("brand_name").toString("");
    appConfig->mApiVodVersion = jsonObject.take("api_vod_version").toString("");
    appConfig->numberOfThread = jsonObject.take("number_of_thread").toInt(5);
    appConfig->mAppVersion = jsonObject.take("app_version").toString();
    appConfig->mDateBuild = jsonObject.take("date_build").toString();
    return appConfig;
}

QJsonObject AppConfig::toJsonObject() {
    QJsonObject jsonObject;
    jsonObject.insert("ping_host", mPingHost);
    jsonObject.insert("socket_url", mSocketUrl);
    jsonObject.insert("base_api_url", mBaseApiUrl);
    jsonObject.insert("login_api_uri", mLoginApiUri);
    jsonObject.insert("token_api_uri", mTokenApiUri);
    jsonObject.insert("cam_site_by_device_api_uri", mCamSiteByDeviceApiUri);
    jsonObject.insert("cam_site_by_site_api_uri", mCamSiteBySiteApiUri);
    jsonObject.insert("base_record_api_url", mBaseRecordApiUrl);
    jsonObject.insert("record_api_uri", mRecordApiUri);
    jsonObject.insert("base_vod_api_url", mBaseVodApiUrl);
    jsonObject.insert("brand_name", mBrandName);
    jsonObject.insert("api_vod_version", mApiVodVersion);
    jsonObject.insert("number_of_thread", numberOfThread);
    jsonObject.insert("app_version", mAppVersion);
    jsonObject.insert("date_build",mDateBuild);
    return jsonObject;
}

QString AppConfig::getBaseApiUrl() const { return mBaseApiUrl; }

void AppConfig::setBaseApiUrl(QString baseApiUrl) { mBaseApiUrl = baseApiUrl; }

QString AppConfig::getBaseRecordApiUrl() const { return mBaseRecordApiUrl; }

void AppConfig::setBaseRecordApiUrl(QString baseRecordApiUrl) {
    mBaseRecordApiUrl = baseRecordApiUrl;
}

QString AppConfig::getBaseVodApiUrl() const { return mBaseVodApiUrl; }

void AppConfig::setBaseVodApiUrl(QString baseVodApiUrl) {
    mBaseVodApiUrl = baseVodApiUrl;
}

QString AppConfig::getCamSiteByDeviceApiUri() const {
    return mCamSiteByDeviceApiUri;
}

void AppConfig::setCamSiteByDeviceApiUri(QString camSiteByDeviceApiUri) {
    mCamSiteByDeviceApiUri = camSiteByDeviceApiUri;
}

QString AppConfig::getCamSiteBySiteApiUri() const {
    return mCamSiteBySiteApiUri;
}

void AppConfig::setCamSiteBySiteApiUri(QString camSiteBySiteApiUri) {
    mCamSiteBySiteApiUri = camSiteBySiteApiUri;
}

QString AppConfig::getPingHost() const { return mPingHost; }

void AppConfig::setPingHost(QString hostPing) { mPingHost = hostPing; }

QString AppConfig::getLoginApiUri() const { return mLoginApiUri; }

void AppConfig::setLoginApiUri(QString loginApiUri) {
    mLoginApiUri = loginApiUri;
}

QString AppConfig::getRecordApiUri() const { return mRecordApiUri; }

void AppConfig::setRecordApiUri(QString recordApiUri) {
    mRecordApiUri = recordApiUri;
}

QString AppConfig::getTokenApiUri() const { return mTokenApiUri; }

void AppConfig::setTokenApiUri(QString tokenApiUri) {
    mTokenApiUri = tokenApiUri;
}

void AppConfig::setChangePasswordApiUri(QString uri) {
    mChangePasswordApiUri = uri;
}

QString AppConfig::getChangePasswordApiUri() const {
    return mChangePasswordApiUri;
}

void AppConfig::setBrandName(QString brandName) {
    this->mBrandName = brandName;
}
QString AppConfig::getBrandName() { return mBrandName; }

void AppConfig::setApiVodVersion(QString apiVersion) {
    mApiVodVersion = apiVersion;
}
QString AppConfig::getApiVodVersion() { return mApiVodVersion; }
