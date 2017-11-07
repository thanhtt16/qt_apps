#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkInterface>
#include <functional>
#include "appprofile.h"

using namespace std;

class NetworkUtils {
 public:
  static bool isConnected;
  static NetworkUtils &instance() {
    static NetworkUtils mInstance;
    return mInstance;
  }
  void getRequest(QString url, std::function<void(QJsonObject)> onSuccess,
                  std::function<void(QString)> onFailure);
  void getRequest(QString uri, QMap<QString, QString> map,
                  std::function<void(QJsonObject)> onSuccess,
                  std::function<void(QString)> onFailure);
  void getRequestVOD(QString uri, QMap<QString, QString> map,
                     std::function<void(QJsonObject)> onSuccess,
                     std::function<void(QString)> onFailure);

  void postRequest(QString uri, QJsonObject json, QMap<QString, QString> map,
                   std::function<void(QJsonObject)> onSuccess,
                   std::function<void(QString)> onFailure);
  void postRequestMonitering(QString uri, QJsonObject json,
                             std::function<void(QJsonObject)> onSuccess,
                             std::function<void(QString)> onFailure);
  bool isConnectedToNetwork();
  QString createParams(QMap<QString, QString> map);

  void fetchToken(function<void(QString, QString)>, function<void(QString)>);
  QString getToken();
  QString getMacAddress();
  void pingToGoogle(function<void()> onConnected,
                    function<void()> onDisconnected);

  QHash<QString, QString> *getHardwareAddresses();

  QString encodeSHA265(QString str);
  NetworkUtils(NetworkUtils const &) = delete;
  NetworkUtils(NetworkUtils &&) = delete;
  NetworkUtils &operator=(NetworkUtils const &) = delete;
  NetworkUtils &operator=(NetworkUtils &&) = delete;

 private:
  qint64 mCallCounter = 0;
  QString token = "";
  QString macAddress = "";
  QHash<QString, QString> *hardwareAddresses;

 protected:
  NetworkUtils();
  ~NetworkUtils();
};

#endif  // NETWORKUTILS_H
