#ifndef P_PBPAGESELECTOR_H
#define P_PBPAGESELECTOR_H

#include <PacModel/presentation.h>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include "Common/LayoutSet.h"
#include "c_pb_pageselector.h"
#include <QSettings>

class C_PBPageSelector;
class P_PBPageSelector : public Presentation {
  // init ui control
 private:
  int numberOfPages = 0;
  int selectedPage = 0;
  int lastSelectedPage = 0;
  LayoutStruct lastSelectedLayout;

  QList<QPushButton *> buttons;
  int max = 100;
  int col = 6;
  int row = 50;
  QGridLayout *pageLayout = Q_NULLPTR;
  QWidget *zone = Q_NULLPTR;
  QWidget *pageWidget = Q_NULLPTR;

  void updateNumberOfPages(int layoutCode);
  C_PBPageSelector *control() { return (C_PBPageSelector *)ctrl; }
  void displayPages();
  void clearPages();
  void displaySelectedPage();
  bool checkPageSaved = false;

 public:
  P_PBPageSelector(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update(LayoutStruct layoutCode);
  void updateDefaultPage(LayoutStruct layoutCode);
  void show(QVariant *attactment);
  QObject *getZone(int zoneId);

  int getNumberOfPages() const;
  void setNumberOfPages(int value);
  int getSelectedPage() const;
  void setSelectedPage(int value);
  void disabledPageButton();
  void enabledPageButton();
 public Q_SLOTS:
  void onPageSelected();
};

#endif  // P_PBPAGESELECTOR_H
