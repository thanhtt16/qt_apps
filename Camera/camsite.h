#ifndef CAMSITE_H
#define CAMSITE_H

#include <QDateTime>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QObject>
#include <QString>
#include <functional>
#include "Authentication/user.h"
#include "Common/LayoutSet.h"
#include "Common/appprofile.h"
#include "Common/networkutils.h"
#include "camitem.h"
#include "camprofile.h"
#include "camstream.h"

class CamItem;
class CamProfile;
class CamStream;

using namespace std;

class CamSite : public QObject {
  Q_OBJECT

 private:
  int sumChannel;
  int layout;
  int playPage;
  int playCamFullscreen;
  QList<CamItem *> camItems;
  // Thao add 24.08.2017
  int totalCamItem;
  int perpageCamItem;

 public:
  static void getCamerasOfSite(int siteId, QString token,
                           std::function<void(CamSite *)> onSuccess,
                           std::function<void(QString)> onFailure);

  static void getCamerasOfSiteWithLayoutPage(int siteId, QString token, int page,
                                       int layout,
                                       std::function<void(CamSite *)> onSuccess,
                                       std::function<void(QString)> onFailure);

  static void fetch(std::function<void(CamSite *)> onSuccess,
                    std::function<void(QString)> onFailure);
  static CamSite *parseWithOrder(bool, QJsonObject jsonObject);
  static CamSite *parseCamItems(QJsonObject jsonObject);
  CamSite(QObject *parent);
  int getSumChannel();
  void setSumChannel(int);
  int getLayout();
  void setLayout(int);
  int getPlayPage();
  void setPlayPage(int);
  int getPlayCamFullScreen();
  QString toString();
  QList<CamItem *> getCamItems();
  void setCamItems(QList<CamItem *> camItems);
  QList<CamItem *> getPageCameras(LayoutStruct layout);
  int getTotalCamItem() const;
  void setTotalCamItem(int value);
};

#endif  // CAMSITE_H
