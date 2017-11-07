#include "p_settings.h"

/**
     * Generic method to override for updating the presention.
     **/

P_Settings::P_Settings(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
  QSize screenSize = Resources::instance().getScreenSize();

  this->zone = zone;
  this->zone->setStyleSheet(
      "background-color: #edf0f5; border: 0px solid #8a8a92; border-radius: "
      "5px;");
  this->zone->resize(screenSize.width() * 0.6f, screenSize.height() * 0.6f);
  this->zone->move((screenSize.width() - this->zone->width()) / 2,
                   (screenSize.height() - this->zone->height()) / 2);

  this->zoneLayout = new QVBoxLayout();
  this->zoneLayout->setMargin(0);
  this->zoneLayout->setSpacing(0);

  this->zone->setLayout(this->zoneLayout);

  QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this->zone);
  effect->setBlurRadius(80);
  effect->setColor(QColor("#000000"));
  effect->setOffset(0, 0);
  this->zone->setGraphicsEffect(effect);

  initTopZone();
  initTitleZone();
  initToolBarZone();
  initCenterZone();

  addZone(":/images/res/icon_tab_general.png", "Chung");
  addZone(":/images/res/icon_tab_videowall.png", "Trực tiếp");
  addZone(":/images/res/icon_tab_playback.png", "Xem lại");
}

void P_Settings::initCenterZone() {
  this->stackWidget = new QStackedWidget(this->zone);
  this->zoneLayout->addWidget(this->stackWidget);
}

void P_Settings::initTopZone() {
  this->topWidget = new QWidget(this->zone);
  this->topWidget->setFixedSize(this->zone->width(), 100);
  this->topWidget->setStyleSheet(
      "background: QLinearGradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 "
      "#eff0f5, stop: 1 #dedede); border: 0px solid #8a8a92; "
      "border-top-left-radius: 5px; border-top-right-radius: 5px; "
      "border-bottom-left-radius: 0px; border-bottom-right-radius: 0px;");
  //  this->topWidget->move(0, 0);
  //  this->topWidget->resize(this->zone->width(), 100);

  this->topLayout = new QVBoxLayout();
  this->topLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  this->topLayout->setSpacing(5);
  this->topLayout->setContentsMargins(0, 0, 0, 0);

  this->topWidget->setLayout(this->topLayout);

  this->zoneLayout->addWidget(this->topWidget);
}

void P_Settings::initTitleZone() {
  titleWidget = new QWidget(this->topWidget);
  titleWidget->setMaximumWidth(this->topWidget->width());
  titleWidget->setStyleSheet("background-color: #00000000");
  titleWidget->installEventFilter(this);
  titleWidget->setMouseTracking(true);
  titleWidget->setAttribute(Qt::WA_Hover);

  QHBoxLayout *titleLayout = new QHBoxLayout();
  titleLayout->setAlignment(Qt::AlignLeft);
  titleLayout->setSpacing(10);
  titleLayout->setContentsMargins(10, 10, 10, 0);
  titleWidget->setLayout(titleLayout);

  this->closeButton =
      createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                   "#aaff3b30", "#ff3b30", "#ff3b30", "");
  this->closeButton->setIcon(QIcon(":/images/res/icon_tab_close.png"));
  this->closeButton->setIconSize(QSize(10, 10));
  connect(this->closeButton, &QPushButton::clicked, this,
          &P_Settings::onCloseButtonClicked);

  this->minimizeButton =
      createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                   "#aaffcc00", "#ffcc00", "#ffcc00", "");
  this->minimizeButton->setIcon(QIcon(":/images/res/icon_tab_minimize.png"));
  this->minimizeButton->setIconSize(QSize(10, 10));
  connect(this->minimizeButton, &QPushButton::clicked, this,
          &P_Settings::onMinimizeButtonClicked);

  this->maximizeButton =
      createButton(titleWidget, QSize(16, 16), QRect(8, 8, 8, 8), 0,
                   "#aa4cd964", "#4cd964", "#4cd964", "");
  this->maximizeButton->setIcon(QIcon(":/images/res/icon_tab_maximize.png"));
  this->maximizeButton->setIconSize(QSize(10, 10));
  connect(this->maximizeButton, &QPushButton::clicked, this,
          &P_Settings::onMaximizeButtonClicked);

  this->titleLabel = new QLabel(titleWidget);
  this->titleLabel->setMinimumWidth(this->topWidget->width() - 110);
  this->titleLabel->setAlignment(Qt::AlignCenter);
  this->titleLabel->setFont(Resources::instance().getLargeBoldButtonFont());
  this->titleLabel->setStyleSheet(
      "background-color: #00000000; color: #1e1e1e");
  this->titleLabel->setText("Cài đặt");

  titleLayout->addWidget(this->closeButton);
  titleLayout->addWidget(this->minimizeButton);
  titleLayout->addWidget(this->maximizeButton);
  titleLayout->addWidget(this->titleLabel);

  this->topLayout->addWidget(titleWidget);
}

void P_Settings::initToolBarZone() {
  this->toolBarWidget = new QWidget(this->topWidget);
  this->toolBarWidget->setStyleSheet("background-color: #00000000;");

  this->toolBarLayout = new QHBoxLayout();
  this->toolBarLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  this->toolBarLayout->setContentsMargins(10, 5, 10, 0);
  this->toolBarLayout->setSpacing(5);

  this->toolBarWidget->setLayout(this->toolBarLayout);

  this->topLayout->addWidget(this->toolBarWidget);
}

void P_Settings::onCloseButtonClicked() { this->hideSettings(); }

void P_Settings::onMinimizeButtonClicked() {}

void P_Settings::onMaximizeButtonClicked() {}

bool P_Settings::eventFilter(QObject *watched, QEvent *event) {
  QWidget *widget = (QWidget *)watched;
  if (widget == titleWidget) {
    QMouseEvent *mouseEvent = (QMouseEvent *)event;
    if (mouseEvent) {
      if (mouseEvent->type() == QMouseEvent::MouseButtonPress) {
        oldPos = mouseEvent->globalPos();
        isMoving = true;
      }
      if (mouseEvent->type() == QMouseEvent::MouseMove) {
        if (isMoving) {
          const QPoint delta = mouseEvent->globalPos() - oldPos;
          this->zone->move(this->zone->x() + delta.x(),
                           this->zone->y() + delta.y());
          oldPos = mouseEvent->globalPos();
        }
      }
      if (mouseEvent->type() == QMouseEvent::MouseButtonRelease) {
        isMoving = false;
      }
    }
  }
  return true;
}

void P_Settings::update() {}

QWidget *P_Settings::getZone(int zoneId) {
  switch (zoneId) {
    case 0:
      return zoneList.at(zoneId);
    case 1:
      return zoneList.at(zoneId);
    case 2:
      return zoneList.at(zoneId);
    default:
      return this->zone;
  }
}
void P_Settings::showSettings(int index) {
  this->zone->show();
  this->zone->raise();
  this->selectTab(index);
}

void P_Settings::hideSettings() {
  this->zone->hide();
  this->zone->raise();
}

void P_Settings::addZone(QString icon, QString title) {
  QToolButton *button = new QToolButton(this->toolBarWidget);
  button->setStyleSheet(
      "QToolButton { color: #1e1e1e; background-color: #00000000; border: 0px "
      "solid #8a8a92; "
      "border-top-left-radius: 5px; border-top-right-radius: 5px; "
      "border-bottom-left-radius: 0px; border-bottom-right-radius: 0px; } "
      "QToolButton QWidget { color: #1e1e1e; }");
  button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  button->setFixedSize(80, 60);
  button->setFont(Resources::instance().getMediumRegularButtonFont());
  button->setIcon(QIcon(icon));
  button->setIconSize(QSize(30, 30));
  button->setText(title);

  connect(button, &QToolButton::clicked, this, &P_Settings::onSelectedTab);
  this->buttonList.append(button);
  this->toolBarLayout->addWidget(button);

  QWidget *widget = new QWidget(this->stackWidget);
  this->zoneList.append(widget);
  this->stackWidget->addWidget(widget);
}

void P_Settings::selectTab(int index) {
  if (index != selectedTabIndex) {
    selectedTabIndex = index;
    stackWidget->setCurrentIndex(selectedTabIndex);
    for (int buttonIndex = 0; buttonIndex < buttonList.size(); ++buttonIndex) {
      QToolButton *button = buttonList.at(buttonIndex);
      if (buttonIndex == selectedTabIndex) {
        button->setStyleSheet(
            "QToolButton { color: #1e1e1e; background-color: #dedede; "
            "border: 0px solid #8a8a92; "
            "border-top-left-radius: 5px; border-top-right-radius: 5px; "
            "border-bottom-left-radius: 0px; border-bottom-right-radius: 0px; "
            "} QToolButton QWidget { color: #1e1e1e; }");
      } else {
        button->setStyleSheet(
            "QToolButton { color: #1e1e1e; background-color: #00000000; "
            "border: 0px solid #8a8a92; "
            "border-top-left-radius: 5px; border-top-right-radius: 5px; "
            "border-bottom-left-radius: 0px; border-bottom-right-radius: 0px; "
            "} QToolButton QWidget { color: #1e1e1e; }");
      }
    }
  }
}

void P_Settings::onSelectedTab() {
  QToolButton *button = (QToolButton *)sender();
  if (button) {
    int index = buttonList.indexOf(button);
    this->selectTab(index);
  }
}
