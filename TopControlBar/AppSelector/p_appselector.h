#ifndef P_APPSELECTOR_H
#define P_APPSELECTOR_H

#include <Authentication/userapp.h>
#include <PacModel/presentation.h>
#include <TopControlBar/AppSelector/c_appselector.h>
#include <QAbstractItemView>
#include <QDebug>
#include <QFont>
#include <QHash>
#include <QHoverEvent>
#include <QLabel>
#include <QListView>
#include <QMouseEvent>
#include <QObject>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QWidget>
class C_AppSelector;
class P_AppSelector : public Presentation {
  // init ui control
 private:
 public:
  QWidget *zone;
  P_AppSelector(Control *ctrl, QWidget *zone);
  // init ui control
  SizeTopControlBar dataSizeTop;

  QListView *mAppsListView;
  C_AppSelector *control() { return (C_AppSelector *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  QWidget *getZone(int zoneId);
  QHash<int, QString> listApps;

  void show();
  void hide();
  void onAppSelected();
  void displayApps();
  void loadDefaultWorkingApp();
  void sizeTopControlBar(QVariant *dataStruct);
 public Q_SLOTS:

 protected:
  void hoverEnter(QHoverEvent *event);
  void hoverLeave(QHoverEvent *event);
  void hoverMove(QHoverEvent *event);
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif  // P_APPSELECTOR_H
