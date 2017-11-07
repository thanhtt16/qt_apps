#include "p_pb_layoutpage.h"

P_PBLayoutPage::P_PBLayoutPage(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  zone->setStyleSheet("background-color: #222");

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setSpacing(20);
  layout->setMargin(10);
  layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  zone->setLayout(layout);

  calendarZone = new QWidget(zone);
  calendarZone->setFixedHeight(300);
  layout->addWidget(calendarZone);

  playListZone = new QWidget(zone);
  playListZone->setFixedHeight(120);
  layout->addWidget(playListZone);

  layoutZone = new QWidget(zone);
  layoutZone->setFixedHeight(100);
  layout->addWidget(layoutZone);

  pageZone = new QWidget(zone);
  layout->addWidget(pageZone);

  showSettingButton = new QPushButton(zone);
  showSettingButton->setText("Cài đặt");
  connect(showSettingButton, &QPushButton::clicked, this, [this] {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(2);
    this->control()->newUserAction(Message.APP_SHOW_SETTINGS, dataStruct);
  });
  layout->addWidget(showSettingButton);
}
void P_PBLayoutPage::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_PBLayoutPage::update() {}

QWidget *P_PBLayoutPage::getZone(int zoneId) {
  switch (zoneId) {
    case Message.APP_PLAY_BACK_ZONE_PAGE_SELECTOR:
      return pageZone;
    case Message.APP_PLAY_BACK_ZONE_LAYOUT_SELECTOR:
      return layoutZone;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_CALENDAR:
      return calendarZone;
    case Message.APP_PLAY_BACK_ZONE_DATA_MAP_PLAYLIST_OF_DAY:
      return playListZone;
    default:
      return Q_NULLPTR;
  }
}
