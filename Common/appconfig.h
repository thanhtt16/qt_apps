#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

class AppConfig : public QObject {
  Q_OBJECT
 private:
  QString mPingHost;
  QString mSocketUrl;
  QString mBaseApiUrl;
  QString mLoginApiUri;
  QString mTokenApiUri;
  QString mCamSiteByDeviceApiUri;
  QString mCamSiteBySiteApiUri;
  QString mBaseRecordApiUrl;
  QString mRecordApiUri;
  QString mBaseVodApiUrl;
  QString mChangePasswordApiUri;
  QString mBrandName;
  QString mApiVodVersion;
  QString mAppVersion;
  QString mDateBuild;
  int numberOfThread = 0;

 public:
  explicit AppConfig(QObject* parent = 0);

  static AppConfig* defaultAppConfig();
  static AppConfig* parseFrom(QJsonObject jsonObject);
  AppConfig* cloneAppConfig();
  QJsonObject toJsonObject();

  QString getPingHost() const;
  void setPingHost(QString hostPing);

  QString getBaseApiUrl() const;
  void setBaseApiUrl(QString baseApiUrl);

  QString getLoginApiUri() const;
  void setLoginApiUri(QString loginApiUri);

  QString getTokenApiUri() const;
  void setTokenApiUri(QString tokenApiUri);

  QString getCamSiteByDeviceApiUri() const;
  void setCamSiteByDeviceApiUri(QString camSiteByDeviceApiUri);

  QString getCamSiteBySiteApiUri() const;
  void setCamSiteBySiteApiUri(QString camSiteBySiteApiUri);

  QString getBaseRecordApiUrl() const;
  void setBaseRecordApiUrl(QString baseRecordApiUrl);

  QString getRecordApiUri() const;
  void setRecordApiUri(QString recordApiUrl);

  QString getBaseVodApiUrl() const;
  void setBaseVodApiUrl(QString baseVodApiUrl);

  QString getChangePasswordApiUri() const;
  void setChangePasswordApiUri(QString uri);

  int getMaxLayoutSize() const;
  void setMaxLayoutSize(int maxLayoutSize);

  int getSlowFps() const;
  void setSlowFps(int slowFps);

  int getStopFps() const;
  void setStopFps(int stopFps);

  int getWaitReplayTimes() const;
  void setWaitReplayTimes(int seconds);

  int getPingInterval() const;
  void setPingInterval(int mseconds);

  int getCheckingInterval() const;
  void setCheckingInterval(int checkingInterval);

  void setBrandName(QString brandName);
  QString getBrandName();
  void setApiVodVersion(QString apiVersion);
  QString getApiVodVersion();
  QString getNetwork() const;
  void setNetwork(const QString& value);

  QString getProtocol() const;
  void setProtocol(const QString& value);

  QString getMainStream() const;
  void setMainStream(const QString& value);

  QString getSubStream() const;
  void setSubStream(const QString& value);

  QString getHostId() const;
  void setHostId(const QString& value);

  QString getSocketUrl() const;
  void setSocketUrl(const QString& socketUrl);

  QString getTvLiveNetwork() const;
  void setTvLiveNetwork(const QString& tvLiveNetwork);

  QString getTvLiveProtocol() const;
  void setTvLiveProtocol(const QString& tvLiveProtocol);

  QString getRtcLiveNetwork() const;
  void setRtcLiveNetwork(const QString& rtcLiveNetwork);

  QString getRtcLiveProtocol() const;
  void setRtcLiveProtocol(const QString& rtcLiveProtocol);

  int getNumberOfThread() const;
  void setNumberOfThread(int value);

  QString getAppVersion() const;
  void setAppVersion(const QString &appVersion);

  QString getDateBuild() const;
  void setDateBuild(const QString &dateBuild);

Q_SIGNALS:

public Q_SLOTS:
};

#endif  // APPCONFIG_H
