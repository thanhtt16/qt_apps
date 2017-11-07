#include "sessionuser.h"
#include <QDebug>
SessionUser::SessionUser(QObject* parent) : QObject(parent) {}

int SessionUser::getPageTransitionDelay() const { return pageTransitionDelay; }

void SessionUser::setPageTransitionDelay(int value) {
  pageTransitionDelay = value;
}

int SessionUser::getPageTransitionStartMode() const {
  return pageTransitionStartMode;
}

void SessionUser::setPageTransitionStartMode(int value) {
  pageTransitionStartMode = value;
}

QString SessionUser::getPathSaveMedia() const { return pathSaveMedia; }

void SessionUser::setPathSaveMedia(const QString& value) {
  pathSaveMedia = value;
}

VideoWallInfo* SessionUser::getVideowall() { return videowall; }

void SessionUser::setVideowall(VideoWallInfo* value) { videowall = value; }

PLaybackInFo* SessionUser::getPlayback() { return playback; }

void SessionUser::setPlayback(PLaybackInFo* value) { playback = value; }

QString SessionUser::getSiteName() const { return siteName; }

void SessionUser::setSiteName(const QString& value) { siteName = value; }

int SessionUser::getIdWorkingSite() const { return idWorkingSite; }

void SessionUser::setIdWorkingSite(int value) { idWorkingSite = value; }

int SessionUser::getIdUser() const { return idUser; }

void SessionUser::setIdUser(int value) { idUser = value; }

SessionUser* SessionUser::createDefaultSessionUser() {
  QJsonObject jsonObject;
  jsonObject.insert("id_user", -1);
  jsonObject.insert("id_working_site", -1);
  jsonObject.insert("site_name", -1);
  jsonObject.insert("path_save_media", "");
  jsonObject.insert("page_transition_start_mode", -1);
  jsonObject.insert("page_transition_delay", -1);

  QJsonObject jsonObjPlayback;
  QJsonArray jsonArr;
  jsonObjPlayback.insert("app_name", "playback");
  jsonObjPlayback.insert("selected_page", -1);
  jsonObjPlayback.insert("selected_layout", -1);
  jsonObjPlayback.insert("seleted_date", "");
  jsonObjPlayback.insert("selected_time_slot", -1);
  jsonObjPlayback.insert("selected_position", -1);
  jsonArr.append(jsonObjPlayback);

  QJsonObject jsonObjVideoWall;
  jsonObjVideoWall.insert("app_name", "videowall");
  jsonObjVideoWall.insert("selected_page", -1);
  jsonObjVideoWall.insert("selected_layout", -1);
  jsonArr.append(jsonObjVideoWall);
  jsonObject.insert("apps", jsonArr);
  SessionUser* defaultSessionUser = SessionUser::parseFrom(jsonObject);
  return defaultSessionUser;
}

QJsonObject SessionUser::toJsonObject() {
  QJsonObject jsonObject;
  jsonObject.insert("id_user", idUser);
  jsonObject.insert("id_working_site", idWorkingSite);
  jsonObject.insert("site_name", siteName);
  jsonObject.insert("path_save_media", pathSaveMedia);
  jsonObject.insert("page_transition_start_mode", pageTransitionStartMode);
  jsonObject.insert("page_transition_delay", pageTransitionDelay);

  QJsonObject jsonObjPlayback;
  QJsonArray jsonArr;
  jsonObjPlayback.insert("app_name", "playback");
  jsonObjPlayback.insert("selected_page", playback->selected_page);
  jsonObjPlayback.insert("selected_layout", playback->selected_layout);
  jsonObjPlayback.insert("seleted_date", playback->selected_date);
  jsonObjPlayback.insert("selected_time_slot", playback->selected_time_slot);
  jsonObjPlayback.insert("selected_position", playback->selected_position);
  jsonArr.append(jsonObjPlayback);

  QJsonObject jsonObjVideoWall;
  jsonObjVideoWall.insert("app_name", "videowall");
  jsonObjVideoWall.insert("selected_page", videowall->selected_page);
  jsonObjVideoWall.insert("selected_layout", videowall->selected_layout);
  jsonArr.append(jsonObjVideoWall);
  jsonObject.insert("apps", jsonArr);

  return jsonObject;
}

SessionUser* SessionUser::parseFrom(QJsonObject jsonObject) {
  SessionUser* sessionUser = new SessionUser(Q_NULLPTR);
  QJsonValue jsonValue;
  jsonValue = jsonObject.take("id_user");
  if (!jsonValue.isNull()) {
    sessionUser->idUser = jsonValue.toInt();
  }
  jsonValue = jsonObject.take("id_working_site");
  if (!jsonValue.isNull()) {
    sessionUser->idWorkingSite = jsonValue.toInt();
  }

  jsonValue = jsonObject.take("site_name");
  if (!jsonValue.isNull()) {
    sessionUser->siteName = jsonValue.toString();
  }
  jsonValue = jsonObject.take("path_save_media");
  if (!jsonValue.isNull()) {
    sessionUser->pathSaveMedia = jsonValue.toString();
  }

  jsonValue = jsonObject.take("page_transition_start_mode");
  if (!jsonValue.isNull()) {
    sessionUser->pageTransitionStartMode = jsonValue.toInt();
  }

  jsonValue = jsonObject.take("page_transition_delay");
  if (!jsonValue.isNull()) {
    sessionUser->pageTransitionDelay = jsonValue.toInt();
  }

  PLaybackInFo* playbackInfo = new PLaybackInFo;
  VideoWallInfo* videowallInfo = new VideoWallInfo;

  jsonValue = jsonObject.take("apps");
  QJsonArray jsonArr = jsonValue.toArray();
  for (int index = 0; index < jsonArr.size(); ++index) {
    if (index == 0) {
      QJsonObject jsonObjPlayback = jsonArr.at(index).toObject();
      if (!jsonObjPlayback.isEmpty()) {
        QJsonValue jsonValuePlayback = jsonObjPlayback.take("selected_page");
        if (!jsonValuePlayback.isNull()) {
          playbackInfo->selected_page = jsonValuePlayback.toInt();
        }
        jsonValuePlayback = jsonObjPlayback.take("selected_layout");
        if (!jsonValuePlayback.isNull()) {
          playbackInfo->selected_layout = jsonValuePlayback.toInt();
        }

        jsonValuePlayback = jsonObjPlayback.take("seleted_date");
        if (!jsonValuePlayback.isNull()) {
          playbackInfo->selected_date = jsonValuePlayback.toString();
        }

        jsonValuePlayback = jsonObjPlayback.take("selected_time_slot");
        if (!jsonValuePlayback.isNull()) {
          playbackInfo->selected_time_slot = jsonValuePlayback.toInt();
        }

        jsonValuePlayback = jsonObjPlayback.take("selected_position");
        if (!jsonValuePlayback.isNull()) {
          playbackInfo->selected_position = jsonValuePlayback.toString();
        }
      }
    } else if (index == 1) {
      QJsonObject jsonObjVideowall = jsonArr.at(index).toObject();
      QJsonValue jsonValueVideoWall = jsonObjVideowall.take("selected_page");
      if (!jsonValueVideoWall.isNull()) {
        videowallInfo->selected_page = jsonValueVideoWall.toInt();
      }
      jsonValueVideoWall = jsonObjVideowall.take("selected_layout");
      if (!jsonValueVideoWall.isNull()) {
        videowallInfo->selected_layout = jsonValueVideoWall.toInt();
      }
    }
  }

  if (playbackInfo) {
    sessionUser->playback = playbackInfo;
  }
  if (videowallInfo) {
    sessionUser->videowall = videowallInfo;
  }

  return sessionUser;
}
