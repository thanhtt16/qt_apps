#include "p_vw_layoutpage.h"

P_VWLayoutPage::P_VWLayoutPage(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  zone->setStyleSheet("background-color: #222; color: white;");

  QVBoxLayout *layout = new QVBoxLayout();
  layout->setSpacing(20);
  layout->setMargin(10);
  layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  zone->setLayout(layout);

  layoutZone = new QWidget(zone);
  layout->addWidget(layoutZone);

  pageZone = new QWidget(zone);
  layout->addWidget(pageZone);

  showSettingButton = new QPushButton(zone);
  showSettingButton->setText("Cài đặt");
  connect(showSettingButton, &QPushButton::clicked, this, [this] {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(1);
    this->control()->getParent()->newAction(Message.APP_SHOW_SETTINGS,
                                            dataStruct);
  });
  layout->addWidget(showSettingButton);
}
void P_VWLayoutPage::show(QVariant *attachment) { Q_UNUSED(attachment) }

void P_VWLayoutPage::update() {}

QWidget *P_VWLayoutPage::getZone(int zoneId) {
  switch (zoneId) {
    case Message.APP_VIDEO_WALL_ZONE_PAGE_SELECTOR:
      return pageZone;
    case Message.APP_VIDEO_WALL_ZONE_LAYOUT_SELECTOR:
      return layoutZone;
    default:
      return Q_NULLPTR;
  }
}
