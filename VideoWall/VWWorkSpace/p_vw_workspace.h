#ifndef P_VW_WORKSPACE_H
#define P_VW_WORKSPACE_H

#include <PacModel/presentation.h>
#include <QEvent>
#include <QObject>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMutex>
#include "Authentication/appcontext.h"
#include "Player/cam9playerpool.h"
#include "c_vw_workspace.h"

class C_VWWorkSpace;

class P_VWWorkSpace : public Presentation {
private:
    int animationDuration = 200;
    QString appName = "Video Wall";
    bool isAnimating = false;
    int selectedIndex = -1;
    QSize videoSize;
    int selectedIndexCurrent = -1;
    QList<CamItem *> pageCameras;
    bool firstRefresh = true;
    CamItemType networkTypeWorking;

public:
    C_VWWorkSpace *control() { return (C_VWWorkSpace *)this->ctrl; }
    P_VWWorkSpace(Control *ctrl, QWidget *zone);

    void stopPlaying();
    void changeControl(Control *ctrl);
    void show(QWidget *zone);
    void update();
    QWidget *getZone(int zoneId);
    void refresh(LayoutStruct layout);
    QWidget *zone = Q_NULLPTR;
    QGridLayout *gridLayout = Q_NULLPTR;
    QList<QWidget *> playerZoneList;
    LayoutStruct selectedLayout;
    LayoutStruct selectedLayoutLast;
//    QList<C_Cam9Player *> players;
    int numberOfPlayingPlayer = 0;
    void networkChangeStatus(int message, QVariant *attachement);

    QList<QWidget *> blackWidgets;
    void refreshAppShow();
    void refreshNewLayoutPage();
    void updateDefaulLayout(LayoutStruct defaultLayout);
    void resizePlayerWhenExitFullScreen();
    void resizePlayerWhenEnterFullScreen();
    void updateNetworkTypeSelected(CamItemType networkType);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

public Q_SLOTS:
    void showFullCellAt(int index);
    void hideFullCellAt(int index);
};

#endif  // PRESENTATION_H
