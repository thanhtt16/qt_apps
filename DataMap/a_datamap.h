#ifndef A_DATAMAP_H
#define A_DATAMAP_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include "PacModel/control.h"
#include "c_datamap.h"

class C_DataMap;

class A_DataMap : public Abstraction {
  Q_OBJECT
 private:
  QNetworkAccessManager *manager;
  QString dataUrl;

 public:
  A_DataMap(Control *ctrl);
  C_DataMap *control() { return (C_DataMap *)this->ctrl; }
  void changeControl(Control *ctrl);
  void loadJsonFrom(QString dataUrl);

  void processDataFrom(QString json);
 public Q_SLOTS:
  void onResponse(QNetworkReply *reply);
};

#endif  // A_DATAMAP_H
