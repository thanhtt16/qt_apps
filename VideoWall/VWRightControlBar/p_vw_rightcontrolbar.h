#ifndef P_VWRIGHTCONTROLBAR_H
#define P_VWRIGHTCONTROLBAR_H

#include <PacModel/presentation.h>
#include <QApplication>
#include <QCalendarWidget>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QListView>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QStackedLayout>
#include <QStringListModel>
#include <QTabWidget>
#include <QTimer>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <cmath>
#include "Common/resources.h"
#include "message.h"

class C_PBRightControlBar;
class P_VWRightControlBar : public Presentation {
 private:
  QTabWidget *tabWidget;
  QWidget *pageLayoutTab;
  QWidget *dataMapTab;
  QWidget *settingTab;

 public:
  QWidget *zone = Q_NULLPTR;

  P_VWRightControlBar(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  QWidget *getZone(int zoneId);
  void show(QVariant *attachment);
};

#endif  // P_VWRIGHTCONTROLBAR_H
