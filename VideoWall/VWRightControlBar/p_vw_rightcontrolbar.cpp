#include "p_vw_rightcontrolbar.h"

P_VWRightControlBar::P_VWRightControlBar(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  this->zone = zone;
  zone->setStyleSheet("background-color: #222");
}

void P_VWRightControlBar::show(QVariant *attachment){Q_UNUSED(attachment)}

QWidget *P_VWRightControlBar::getZone(int zoneId) {
  switch (zoneId) {
    case Message.APP_VIDEO_WALL_ZONE_LAYOUT_PAGE:
      return this->zone;

    case Message.APP_VIDEO_WALL_ZONE_SETTING:
      return settingTab;

    default:
      return Q_NULLPTR;
  }
}
