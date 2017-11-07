#ifndef P_PROFILESETTING_H
#define P_PROFILESETTING_H

#include <PacModel/presentation.h>
#include <TopControlBar/ProfileSetting/c_profilesetting.h>
#include <QAbstractItemView>
#include <QFont>
#include <QListView>
#include <QObject>
#include <QStringListModel>
#include <QVBoxLayout>
#include "Common/generaldefine.h"
#include <Common/resources.h>
#include "Common/appconfig.h"
#include "Common/appprofile.h"
class C_ProfileSetting;
class P_ProfileSetting : public Presentation {
  // init ui control
 private:
 public:
  QWidget *zone;
  SizeTopControlBar dataSizeTop;
  P_ProfileSetting(Control *ctrl, QWidget *zone);
  // init ui control
  QListView *mProfileListView;
  C_ProfileSetting *control() { return (C_ProfileSetting *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  QWidget *getZone(int zoneId);
  void show();
  void hide();
  void sizeTopControlBar(QVariant *dataStruct);
 private Q_SLOTS:
  void onExitButtonClicked();
  void onLogoutButtonClicked();
  void onSettingClicked();
  void onAboutButtonClicked();

 protected:
  void hoverEnter(QHoverEvent *event);
  void hoverLeave(QHoverEvent *event);
  void hoverMove(QHoverEvent *event);
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif  // P_PROFILESETTING_H
