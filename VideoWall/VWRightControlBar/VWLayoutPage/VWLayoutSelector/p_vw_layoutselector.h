#ifndef P_VWLAYOUTSELECTOR_H
#define P_VWLAYOUTSELECTOR_H

#include <PacModel/presentation.h>
#include <QFont>
#include <QGridLayout>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <cmath>
#include "Common/LayoutSet.h"
#include "c_vw_layoutselector.h"
#include "message.h"
#include "Authentication/appcontext.h"
#include <QSettings>
class C_VWLayoutSelector;

class P_VWLayoutSelector : public Presentation {
    // init ui control
private:
    QList<QPushButton *> buttons;
    int max = 5;
    int selectedButtonIndex = -1;
    QHBoxLayout *layout = Q_NULLPTR;
    LayoutSet *layoutSet = Q_NULLPTR;

    bool checkLayoutSaved = false;
public:
    P_VWLayoutSelector(Control *ctrl, QWidget *zone);
    C_VWLayoutSelector *control() { return (C_VWLayoutSelector *)this->ctrl; }
    void changeControl(Control *ctrl);
    void update();
    void show(QVariant *attactment);
    QObject *getZone(int zoneId);
    void clearOldSelectedButton(int buttonIndex);
    void highlightNewSelectedButton(int buttonIndex);
    void setDefaultLayout();
    void calibrateLayoutSet();

    bool isWaiting = false;
    void startWaiting();
    QPushButton *lastButton = Q_NULLPTR;
    QTimer *waitingTimer;

public Q_SLOTS:
    void onSelectedLayout();
    void onEndWaiting();
};

#endif  // P_VWLAYOUTSELECTOR_H
