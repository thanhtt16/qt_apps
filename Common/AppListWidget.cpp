#include "AppListWidget.h"
#include <QDebug>

AppListWidget::AppListWidget(QWidget *parent) : QWidget(parent) {
  initSelectionAppNameWidget();
}

void AppListWidget::initSelectionAppNameWidget() {
  mSelectionWidget = new QWidget(this);
  mSelectionWidget->resize(250, 340);
  mSelectionWidget->setMaximumSize(500, 500);
  mSelectionWidget->setMinimumSize(0, 0);
  mSelectionLayout = new QVBoxLayout();
  mSelectionLayout->setMargin(10);
  mSelectionLayout->setSpacing(20);
  mSelectionLayout->setAlignment(Qt::AlignCenter);
  mSelectionWidget->setLayout(mSelectionLayout);
  mSelectionWidget->setStyleSheet("background:#FFFFFF");
  mAppNameListView = new QListView(mSelectionWidget);
  connect(mAppNameListView, &QListView::clicked, this,
          &AppListWidget::valueChangedAppName);
  mAppNameListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //    mAppNameListView->setStyleSheet(utils.borderStyleSheet(colors.WHITE,
  //    colors.MIDNIGHT_BLUE, 0, 1));
  mAppNameModel = new QStringListModel();
  mListAppName.append("Video Wall");
  mListAppName.append("Magic Show");
  mAppNameModel->setStringList(mListAppName);
  mAppNameListView->setSpacing(4);
  QFont fontListView = mAppNameListView->font();
  fontListView.setPointSize(13);
  //    fontTab.setBold(true);
  mAppNameListView->setFont(fontListView);
  mAppNameListView->setModel(mAppNameModel);
  mSelectionLayout->addWidget(mAppNameListView);
}

void AppListWidget::valueChangedAppName() {
  int row = mAppNameListView->selectionModel()->selectedRows().first().row();
  QString appName = mListAppName.at(row);
  Q_EMIT selectApp(appName);
}

void AppListWidget::updateModelListView(QStringList listName) {
  Q_UNUSED(listName)
  //    if(mListAppNameName.size() != 0) mListAppNameName.clear();
  //    mListAppNameName = listName;
  //    mAppNameModel->setStringList(mListAppNameName);
}

// void AppListWidget::setChangeAppName(AppListWidget *changeAppName)
//{
//    this->mChangeAppName = changeAppName;
//}

// void AppListWidget::setListAppNameId(QList<int> listAppNameId)
//{
//    if(mListAppNameId.size() != 0) mListAppNameId.clear();
//    this->mListAppNameId = listAppNameId;
//}
