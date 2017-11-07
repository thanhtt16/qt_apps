#include "networkutils.h"

bool NetworkUtils::isConnected = false;

void NetworkUtils::getRequest(QString uri, QMap<QString, QString> params,
                              function<void(QJsonObject)> onSuccess,
                              function<void(QString)> onFailure) {
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QString url = AppProfile::getAppProfile()->getAppConfig()->getBaseApiUrl();
  url.append(uri).append(createParams(params));
  qDebug() << "url - REQUEST " << url;
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  eventLoop.exec();
  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = document.object();
    onSuccess(jsonObject);
    delete reply;
  } else {
    //    qDebug() << "Lỗi yêu cầu:" << reply->errorString();
    onFailure(reply->errorString());
    delete reply;
  }
}

void NetworkUtils::getRequest(QString url,
                              function<void(QJsonObject)> onSuccess,
                              function<void(QString)> onFailure) {
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  eventLoop.exec();
  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = document.object();
    onSuccess(jsonObject);

    delete reply;
  } else {
    onFailure(reply->errorString());
    delete reply;
  }
}

void NetworkUtils::getRequestVOD(QString uri, QMap<QString, QString> params,
                                 function<void(QJsonObject)> onSuccess,
                                 function<void(QString)> onFailure) {
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QString url = uri.append(createParams(params));
  qDebug() << Q_FUNC_INFO << url;
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  eventLoop.exec();
  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = document.object();
    qDebug() << Q_FUNC_INFO << jsonObject;
    onSuccess(jsonObject);  // giá trị trả về nếu thànhcông
    delete reply;
  } else {
    onFailure(reply->errorString());
    delete reply;
  }
}

void NetworkUtils::postRequest(QString uri, QJsonObject json,
                               QMap<QString, QString> params,
                               std::function<void(QJsonObject)> onSuccess,
                               std::function<void(QString)> onFailure) {
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QString url = AppProfile::getAppProfile()->getAppConfig()->getBaseApiUrl();
  url.append(uri).append(createParams(params));
  qDebug() << Q_FUNC_INFO << url;
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QNetworkReply *reply = manager.post(request, QJsonDocument(json).toJson());
  eventLoop.exec();

  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = document.object();
    onSuccess(jsonObject);
    delete reply;
  } else {
    onFailure(reply->errorString());
    delete reply;
  }
}

void NetworkUtils::postRequestMonitering(
    QString uri, QJsonObject json, std::function<void(QJsonObject)> onSuccess,
    std::function<void(QString)> onFailure) {
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QNetworkRequest request(uri);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  QNetworkReply *reply = manager.post(request, QJsonDocument(json).toJson());
  eventLoop.exec();
  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = document.object();
    onSuccess(jsonObject);
    delete reply;
  } else {
    onFailure(reply->errorString());
    delete reply;
  }
}

bool NetworkUtils::isConnectedToNetwork() {
  QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
  bool result = false;
  for (int i = 0; i < ifaces.count(); i++) {
    QNetworkInterface iface = ifaces.at(i);
    if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
        !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
      for (int j = 0; j < iface.addressEntries().count(); j++) {
        if (result == false) {
          result = true;
        }
      }
    }
  }
  return result;
}

void NetworkUtils::pingToGoogle(function<void()> onConnected,
                                function<void()> onDisconnected) {
  QNetworkAccessManager nam;
  QNetworkRequest req(
      QUrl(AppProfile::getAppProfile()->getAppConfig()->getPingHost()));
  QNetworkReply *reply = nam.get(req);
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();
  if (reply->bytesAvailable()) {
    onConnected();
    reply->deleteLater();
  } else {
    onDisconnected();
    reply->deleteLater();
  }
}

QString NetworkUtils::createParams(QMap<QString, QString> params) {
  QString str = "";
  if (params.size() > 0) {
    str = "?";
    QMap<QString, QString>::iterator i = params.begin();
    while (i != params.end()) {
      str.append(i.key());
      str.append("=");
      str.append(i.value());
      str.append("&");
      i++;
    }
    str.remove(str.size() - 1, 1);
  }
  return str;
}

QHash<QString, QString> *NetworkUtils::getHardwareAddresses() {
  return hardwareAddresses;
}

void NetworkUtils::fetchToken(function<void(QString, QString)> onSuccess,
                              function<void(QString)> onFailure) {
  function<void(QJsonObject)> onFetchSuccess =
      [this, onSuccess, onFailure](QJsonObject jsonObject) {
        int code = jsonObject.value("code").toInt(-1);
        if (code == 0) {
          QString message = jsonObject.value("message").toString();
          token = jsonObject.value("token").toString();
          onSuccess(message, token);
        } else {
          QString status = jsonObject.value("status").toString();
          onFailure(status);
        }
      };
  function<void(QString)> onFetchFailure = [onFailure](QString message) {
    onFailure(message);
  };
  QMap<QString, QString> params;
  if (!hardwareAddresses->isEmpty()) {
    macAddress = hardwareAddresses->values().first();
  }
  params.insert("device_id", macAddress);
  getRequest(AppProfile::getAppProfile()->getAppConfig()->getTokenApiUri(),
             params, onFetchSuccess, onFetchFailure);
}

QString NetworkUtils::getToken() { return token; }

QString NetworkUtils::getMacAddress() { return macAddress; }

QString NetworkUtils::encodeSHA265(QString str) {
  return QString(
      QCryptographicHash::hash((str.toUtf8()), QCryptographicHash::Sha256)
          .toHex());
}

NetworkUtils::NetworkUtils() {
  hardwareAddresses = new QHash<QString, QString>();
  for (int index = 0; index < QNetworkInterface::allInterfaces().size();
       ++index) {
    QNetworkInterface networkInterface =
        QNetworkInterface::allInterfaces().at(index);
    QString hardwareAddress = networkInterface.hardwareAddress();
    if (hardwareAddress != "00:00:00:00:00:00") {
      QNetworkInterface::InterfaceFlags flags = networkInterface.flags();
      if (flags.testFlag(QNetworkInterface::IsUp) &&
          flags.testFlag(QNetworkInterface::IsRunning) &&
          flags.testFlag(QNetworkInterface::CanBroadcast) &&
          flags.testFlag(QNetworkInterface::CanMulticast)) {
        hardwareAddresses->insert(networkInterface.name(), hardwareAddress);
      }
    }
  }
}

NetworkUtils::~NetworkUtils() { delete hardwareAddresses; }
