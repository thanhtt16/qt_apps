#ifndef P_MAINFRAME_H
#define P_MAINFRAME_H

#include <PacModel/presentation.h>
#include <Site/changesite.h>
#include <QObject>
#include <QStackedLayout>
#include <QWidget>
#include "c_mainframe.h"
#include "message.h"

class C_MainFrame;
class P_MainFrame : public Presentation {
 public:
  QWidget *zone = Q_NULLPTR;
  QWidget *topBar = Q_NULLPTR;
  QWidget *topBarOverlay = Q_NULLPTR;
  QWidget *workSpaceForVideoWall = Q_NULLPTR;
  QWidget *workSpaceForMagicShow = Q_NULLPTR;
  QWidget *workSpaceForPlayBack = Q_NULLPTR;
  QWidget *loadingIndicator = Q_NULLPTR;
  QStackedLayout *workSpaceLayout = Q_NULLPTR;
  ChangeSite *siteTop;
  C_MainFrame *control() { return (C_MainFrame *)this->ctrl; }
  QWidget *settingZone = Q_NULLPTR;
  QWidget *aboutApp = Q_NULLPTR;

 private:
  AppSize appSize;

 public:
  bool isTopBarOverlayCanHide = true;
  P_MainFrame(Control *ctrl, QWidget *zone);
  void changeControl(Control *ctrl);
  void update();
  QWidget *getZone(int zoneId);
  void showApp(int message);

  void enterFullscreenMode();
  void exitFullscreenMode();

  void topBarOverlayCanHide();
  void topBarOverlayCanNotHide();

 protected:
  void hoverEnter(QHoverEvent *event);
  void hoverLeave(QHoverEvent *event);
  void hoverMove(QHoverEvent *event);
  bool eventFilter(QObject *watched, QEvent *event);
};

#endif  // P_MAINFRAME_H
