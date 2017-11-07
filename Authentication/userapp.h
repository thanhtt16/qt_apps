#ifndef USERAPP_H
#define USERAPP_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

struct UserApp {
  QString appName;
  int appCode;
  QString appIcon;
};
Q_DECLARE_METATYPE(UserApp)

#endif  // USERAPP_H
