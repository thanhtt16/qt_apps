#include "ms_profilepage.h"

ProfilePage::ProfilePage(QObject *parent) : QObject(parent) {
  mPage = 1;
  mDuration = 0;
  mLayout = 4;
}

void ProfilePage::setIds(QList<int> ids) { mCamIds = ids; }

ProfilePage *ProfilePage::parseFrom(QJsonObject jsonObject) {
  ProfilePage *profilePage = new ProfilePage(NULL);
  QJsonValue jsonValue;

  jsonValue = jsonObject.take("layout");
  if (!jsonValue.isNull()) {
    profilePage->mLayout = jsonValue.toInt(0);
  }

  jsonValue = jsonObject.take("page");
  if (!jsonValue.isNull()) {
    profilePage->mPage = jsonValue.toInt(0);
  }

  jsonValue = jsonObject.take("duration");
  if (!jsonValue.isNull()) {
    profilePage->mDuration = jsonValue.toInt(60);
  }

  QJsonArray jsonArray = jsonObject["ids"].toArray();
  QList<int> ids;
  for (int index = 0; index < jsonArray.size(); ++index) {
    ids.append(jsonArray[index].toInt());
  }
  profilePage->setIds(ids);
  return profilePage;
}

QJsonObject ProfilePage::toJsonObject() {
  QJsonObject json;
  json.insert("layout", mLayout);
  json.insert("page", mPage);
  json.insert("duration", mDuration);
  QJsonArray ids;
  for (int index = 0; index < mCamIds.size(); ++index) {
    int id = mCamIds.at(index);
    ids.append(id);
  }
  json.insert("ids", ids);
  return json;
}

QList<int> ProfilePage::getCamIds() { return mCamIds; }

QList<CamItem *> ProfilePage::getCamItems() { return mCamItems; }

void ProfilePage::setLayout(int newLayout) {
  mLayout = newLayout;
  int delta = mLayout - mCamIds.size();
  if (delta > 0) {
    for (int index = 0; index < delta; ++index) {
      mCamIds.append(0);
    }
  } else {
    for (int index = 0; index < delta; ++index) {
      mCamIds.removeLast();
    }
  }
}

int ProfilePage::getLayout() const { return mLayout; }

void ProfilePage::setPage(int newPage) { mPage = newPage; }

int ProfilePage::getPage() const { return mPage; }

void ProfilePage::setDuration(int duration) { mDuration = duration; }

int ProfilePage::getDuration() const { return mDuration; }

CamSite *ProfilePage::toCamSite(CamSite *sourceCamSite) {
  CamSite *camSite = new CamSite(NULL);
  //  QList<CamItem *> *camItems = new QList<CamItem *>();
  //  QList<CamItem *> *sourceCamItems = sourceCamSite->getCamItems();
  //  for (int index = 0; index < mCamIds.size(); ++index) {
  //    CamItem *camItem = new CamItem(sourceCamSite);
  //    camItems->append(camItem);
  //  }
  //  int size = sourceCamItems->size();
  //  for (int index = 0; index < size; ++index) {
  //    CamItem *camItem = sourceCamItems->at(index);
  //    int id = camItem->getCameraId();
  //    for (int jndex = 0; jndex < mCamIds.size(); ++jndex) {
  //      if (id == mCamIds.at(jndex)) {
  //        camItem->setOrder(jndex);
  //        camItems->replace(jndex, camItem);
  //        break;
  //      }
  //    }
  //  }
  //  camSite->setSumChannel(camItems->size());
  //  camSite->setLayout(mLayout);
  //  camSite->setPlayPage(1);
  //  camSite->setCamItems(camItems);
  return camSite;
}
