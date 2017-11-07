#ifndef P_VWPAGESELECTOR_H
#define P_VWPAGESELECTOR_H

#include <PacModel/presentation.h>
#include <QGridLayout>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QScrollArea>
#include <QWidget>
#include "Common/LayoutSet.h"
#include "c_vw_pageselector.h"
#include <QSettings>

class C_VWPageSelector;
class P_VWPageSelector : public Presentation {
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
    bool checkPageSaved = false;
    void updateNumberOfPages(int layoutCode);
    C_VWPageSelector *control() { return (C_VWPageSelector *)ctrl; }
    void displayPages();
    void clearPages();
    void displaySelectedPage();

    bool isWaiting = false;
    void startWaiting();
    QPushButton *lastButton = Q_NULLPTR;
    QTimer *waitingTimer;

public:
    P_VWPageSelector(Control *ctrl, QWidget *zone);
    void changeControl(Control *ctrl);
    void update(LayoutStruct layoutCode);
    void updateDefaulPage(LayoutStruct layoutCode);
    void show(QVariant *attactment);
    QObject *getZone(int zoneId);

    int getNumberOfPages() const;
    void setNumberOfPages(int value);
    int getSelectedPage() const;
    void setSelectedPage(int value);
    void pageTransitionBegin();
public Q_SLOTS:
    void onPageSelected();
    void onEndWaiting();
};

#endif  // P_VWPAGESELECTOR_H
