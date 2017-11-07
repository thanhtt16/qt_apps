#ifndef P_PBLAYOUTSELECTOR_H
#define P_PBLAYOUTSELECTOR_H

#include <PacModel/presentation.h>
#include <QFont>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <cmath>
#include "Common/LayoutSet.h"
#include "c_pb_layoutselector.h"
#include "message.h"
#include "Authentication/appcontext.h"
#include <QSettings>
class C_PBLayoutSelector;

class P_PBLayoutSelector : public Presentation {
  // init ui control
 private:
  QList<QPushButton *> buttons;
  int max = 5;
  int selectedButtonIndex = -1;
  QHBoxLayout *layout = Q_NULLPTR;
  LayoutSet *layoutSet = Q_NULLPTR;

  bool checkLayoutSaved = false;

 public:
  P_PBLayoutSelector(Control *ctrl, QWidget *zone);
  C_PBLayoutSelector *control() { return (C_PBLayoutSelector *)this->ctrl; }
  void changeControl(Control *ctrl);
  void update();
  void show(QVariant *attactment);
  QObject *getZone(int zoneId);
  void clearOldSelectedButton(int buttonIndex);
  void highlightNewSelectedButton(int buttonIndex);
  void setDefaultLayout();
  void calibrateLayoutSet();
  void setDefaultDate();
  void enabledLayoutButton();
  void disabledLayoutButton();
 public Q_SLOTS:
  void onSelectedLayout();
};

#endif  // P_PBLAYOUTSELECTOR_H
