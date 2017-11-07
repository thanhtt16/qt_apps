#include "p_appselector.h"

/**
     * Generic method to override for updating the presention.
     **/

P_AppSelector::P_AppSelector(Control *ctrl, QWidget *zone)
    : Presentation(ctrl) {
  this->zone = zone;
  this->zone->hide();
  this->zone->setStyleSheet(
      "background-color: #FFFFFF; color: black; border:0px;");
  this->zone->setFixedSize(150, 80);

  QVBoxLayout *mSelectionLayout = new QVBoxLayout();
  mSelectionLayout->setMargin(10);
  mSelectionLayout->setSpacing(20);
  mSelectionLayout->setAlignment(Qt::AlignCenter);
  this->zone->setLayout(mSelectionLayout);

  mAppsListView = new QListView(this->zone);
  displayApps();
  mAppsListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  mAppsListView->setStyleSheet("background-color: white; color: black;");
  mAppsListView->setStyleSheet(
      "QListView::item:hover{background-color:#9bdcd0}");
  mAppsListView->setSpacing(4);

  mAppsListView->setFont(Resources::instance().getLargeRegularButtonFont());

  connect(mAppsListView, &QListView::clicked, this,
          &P_AppSelector::onAppSelected);

  mSelectionLayout->addWidget(mAppsListView);

  mAppsListView->setMouseTracking(true);
  mAppsListView->setAttribute(Qt::WA_Hover);
  mAppsListView->installEventFilter(this);
}

void P_AppSelector::update() {}

void P_AppSelector::show() {
  if (control()->zone->isVisible()) {
    control()->zone->hide();
  } else {
    control()->zone->show();

    control()->zone->raise();
  }
}

void P_AppSelector::hide() { control()->zone->hide(); }

void P_AppSelector::onAppSelected() {
  QList<UserApp> userApps = control()->appContext->getUserApps();

  int selectedRow =
      mAppsListView->selectionModel()->selectedRows().first().row();

  UserApp selectedApp = userApps.at(selectedRow);
  UserApp appWorking = control()->appContext->getWorkingApp();
  if (selectedApp.appCode == appWorking.appCode)
    hide();
  else {
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue(selectedApp);
    control()->newUserAction(Message.APP_NEW_SELECTED, dataStruct);
    this->hide();
  }
}

void P_AppSelector::loadDefaultWorkingApp() {
  QList<UserApp> userApps = control()->appContext->getUserApps();
  UserApp defaultApp = userApps.at(0);
  QVariant *dataStruct = new QVariant();
  dataStruct->setValue(defaultApp);
  control()->newUserAction(Message.APP_NEW_SELECTED, dataStruct);
}

void P_AppSelector::displayApps() {
  QList<UserApp> userApps = control()->appContext->getUserApps();
  QStringList listApps;
  for (int index = 0; index < userApps.size(); ++index) {
    listApps.append(userApps.at(index).appIcon + "   " +
                    userApps.at(index).appName);
  }
  QStringListModel *listModel = new QStringListModel();
  listModel->setStringList(listApps);
  mAppsListView->setModel(listModel);
}

QWidget *P_AppSelector::getZone(int zoneId) {
  switch (zoneId) {
    case 1:
      return Q_NULLPTR;
    default:
      return Q_NULLPTR;
  }
}

bool P_AppSelector::eventFilter(QObject *watched, QEvent *event) {
  QListView *listview = qobject_cast<QListView *>(watched);
  if (listview == mAppsListView) {
    switch (event->type()) {
      case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent *>(event));
        return true;
        break;
      case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent *>(event));
        return true;
        break;
      case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent *>(event));
        return true;
        break;
      default:
        break;
    }
  }

  //  return QWidget::eventFilter(watched, event);
  return false;
}

void P_AppSelector::hoverEnter(QHoverEvent *) {}

void P_AppSelector::hoverLeave(QHoverEvent *) {
  control()->newUserAction(Message.APP_SELECTOR_HIDE_ALL, Q_NULLPTR);
  control()->zone->hide();
}

void P_AppSelector::hoverMove(QHoverEvent *) { /* qDebug() << "hover move";*/
}

void P_AppSelector::sizeTopControlBar(QVariant *dataStruct) {
  dataSizeTop = dataStruct->value<SizeTopControlBar>();
  int widthMove = dataSizeTop.widthBrand + 65 + dataSizeTop.widthSite;
  // init gui object
  this->zone->setFixedSize(150, 120);
  this->zone->move(widthMove, 40);
}
