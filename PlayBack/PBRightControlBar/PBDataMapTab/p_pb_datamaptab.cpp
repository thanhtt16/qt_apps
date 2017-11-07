#include "p_pb_datamaptab.h"

P_PBDataMapTab::P_PBDataMapTab(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  //  zone->setStyleSheet("background: #");
  QVBoxLayout *layout = new QVBoxLayout();
  layout->setMargin(15);
  layout->setSpacing(20);
  layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  zone->setLayout(layout);

  calendarZone = new QWidget(zone);
  calendarZone->setFixedHeight(600);
  layout->addWidget(calendarZone);

  playlistZone = new QWidget(zone);
  layout->addWidget(playlistZone);
}
void P_PBDataMapTab::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_PBDataMapTab::update() {}

QWidget *P_PBDataMapTab::getZone(int zoneId) {
  switch (zoneId) {
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR:
      return calendarZone;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY:
      return playlistZone;
    default:
      return Q_NULLPTR;
  }
}
