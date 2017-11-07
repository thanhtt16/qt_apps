#include "p_pb_rightcontrolbar.h"

P_PBRightControlBar::P_PBRightControlBar(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  tabWidget = new QTabWidget(zone);
  QStackedLayout *layout = new QStackedLayout();
  zone->setLayout(layout);
  layout->addWidget(tabWidget);
  layout->setCurrentWidget(tabWidget);

  pageLayoutTab = new QWidget(tabWidget);
  //  pageLayoutTab->setStyleSheet("background-color: red");
  tabWidget->addTab(pageLayoutTab, "Trang");

  dataMapTab = new QWidget(tabWidget);
  //  dataMapTab->setStyleSheet("background-color: yellow");
  tabWidget->addTab(dataMapTab, "Data Map");

  //  settingTab = new QWidget(tabWidget);
  //  //  settingTab->setStyleSheet("background-color: blue");
  //  tabWidget->addTab(settingTab, "Cài đặt");
  tabWidget->setStyleSheet(
      "QTabBar::tab{font-family:Arial;color:#8a8e91;background:#F7F7F7;border-"
      "left:1px solid #D6D9DC;border-right:0.5px solid #D6D9DC;border-top:1px "
      "solid #9d9c9d;border-bottom:1px solid "
      "#BFC4C9;border-top-left-radius:0px;border-top-right-radius:0px;width:"
      "98px;height:29px;padding:0px}QTabBar::tab:selected{background:#489BF7}"
      "QTabBar::tab:selected{border-color:#9B9B9B #9B9B9B "
      "#C2C7CB;color:#FFFFFF}QTabBar::tab:!selected{margin-top:0px}QTabWidget::"
      "pane{border-top:0px solid #C2C7CB}QTabWidget::tab-bar{left:0px}");
}

void P_PBRightControlBar::show(QVariant *attachment){Q_UNUSED(attachment)}

QWidget *P_PBRightControlBar::getZone(int zoneId) {
  switch (zoneId) {
    case Message.APP_PLAY_BACK_ZONE_LAYOUT_PAGE:
      return pageLayoutTab;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP:
      return dataMapTab;
    case Message.APP_PLAY_BACK_ZONE_SETTING:
      return settingTab;
    default:
      return Q_NULLPTR;
  }
}
