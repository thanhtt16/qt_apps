#include "a_datamap.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_DataMap::A_DataMap(Control* ctrl) : Abstraction(ctrl) {
  manager = new QNetworkAccessManager(this);
  connect(manager, &QNetworkAccessManager::finished, this,
          &A_DataMap::onResponse);
}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_DataMap::changeControl(Control* ctrl) { this->ctrl = ctrl; }

void A_DataMap::loadJsonFrom(QString dataUrl) {
  this->dataUrl = dataUrl;
  qDebug() << "load json from" << this->dataUrl;
  manager->get(QNetworkRequest(QUrl(this->dataUrl)));
}

void A_DataMap::onResponse(QNetworkReply* reply) {
  QString json = reply->readAll();
  processDataFrom(json);
}

void A_DataMap::processDataFrom(QString json) {
//  QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
//  QJsonObject jsonObject = jsonDocument.object();
//  dataMapArray = new DataMapArray(this);
//  dataMapArray->parseFrom(jsonObject);
//  QVariant* dataStruct = new QVariant();
//  dataStruct->setValue(dataMapArray);
//  control()->newSystemAction(Message.APP_PLAY_BACK_DATA_MAP_DRAW, dataStruct);
}
