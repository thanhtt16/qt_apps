#include "changesite.h"
#include <QDebug>

ChangeSite::ChangeSite(QWidget *parent) : QWidget(parent) {
  initSelectionSiteWidget();
}

void ChangeSite::initSelectionSiteWidget() {
  setFixedSize(300, 400);
  setStyleSheet("#dddddd");
  //    mainVBoxLayout = new QVBoxLayout();
  //    mainVBoxLayout->setMargin(0);
  //    mainVBoxLayout->setSpacing(0);
  //    setLayout(mainVBoxLayout);

  mSelectionWidget = new QWidget(this);
  mSelectionWidget->setFixedSize(250, 340);
  mSelectionLayout = new QVBoxLayout();
  mSelectionLayout->setMargin(10);
  mSelectionLayout->setSpacing(20);
  mSelectionLayout->setAlignment(Qt::AlignCenter);
  mSelectionWidget->setLayout(mSelectionLayout);
  mSelectionWidget->setStyleSheet("background:#FFFFFF");
  //    mainVBoxLayout->addWidget(mSelectionWidget);

  //    mSearchWidget = new QWidget(mSelectionWidget);
  //    mSearchLayout = new QVBoxLayout();
  //    mSearchWidget->setLayout(mSearchLayout);
  //    mSelectionLayout->addWidget(mSearchWidget);
  //    mSearchWidget->setStyleSheet("background:#EFEFEF");

  //    mSearchLineEdit = new QLineEdit(mSearchWidget);
  //    mSearchLineEdit->setMaximumHeight(30);
  //    mSearchLineEdit->setMinimumHeight(30);
  //    mSearchLineEdit->setStyleSheet(utils.borderStyleSheet(colors.WHITE,
  //    "#4f4f4f", 0, 15));
  //    mSearchLineEdit->setTextMargins(5,2,5,2);
  //    mSearchLineEdit->setPlaceholderText("Tìm kiếm...");
  //    mSearchLayout->addWidget(mSearchLineEdit);
  //    connect(mSearchLineEdit, &QLineEdit::textChanged, this,
  //    &ProfileGridWidget::filterWithKeyword);

  mSiteListView = new QListView(mSelectionWidget);
  connect(mSiteListView, &QListView::clicked, this,
          &ChangeSite::valueChangedSite);
  mSiteListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //  mSiteListView->setStyleSheet(
  //      utils.borderStyleSheet(colors.WHITE, colors.MIDNIGHT_BLUE, 0, 1));
  mSiteModel = new QStringListModel();
  mSiteListView->setSpacing(4);
  QFont fontListView = mSiteListView->font();
  fontListView.setPointSize(13);
  //    fontTab.setBold(true);
  mSiteListView->setFont(fontListView);
  mSiteListView->setModel(mSiteModel);
  mSelectionLayout->addWidget(mSiteListView);
}

void ChangeSite::valueChangedSite() {
  int row = mSiteListView->selectionModel()->selectedRows().first().row();
  QString siteName = mListSiteName.at(row);
  int siteId = mListSiteId.at(row);
  Q_EMIT selectSite(siteName, siteId, row);
}

void ChangeSite::updateModelListView(QStringList listName) {
  QStringListModel *listModel = new QStringListModel();
  listModel->setStringList(listName);
  mSiteListView->setModel(listModel);
}
void ChangeSite::setListSites(SiteChild *childSite) {
  QStringList listSites;
  listSites.append(childSite->getSiteName());
  for (int index = 0; index < childSite->getListSite().size(); ++index) {
    listSites.append(childSite->getListSite().at(index)->getSiteName());
  }
  //  if (mListSiteName.size() != 0) mListSiteName.clear();
  mListSiteName = listSites;
  updateModelListView(mListSiteName);
}
void ChangeSite::startSearch() {
  if (!mSearchTimer->isActive()) {
    connect(mSearchTimer, &QTimer::timeout, this, &ChangeSite::onSearch);
    mSearchTimer->start(300);
  }
}

void ChangeSite::setChildSite(SiteChild *childSite) {
  //  this->childSite = childSite;
  QStringList names;
  QList<int> ids;
  names.append(childSite->getSiteName());
  ids.append(childSite->getSiteId());
  QList<Site *> sites = childSite->getListSite();
  for (int index = 0; index < sites.size(); ++index) {
    Site *site = sites.at(index);
    names.append(site->getSiteName());
    ids.append(site->getSiteId());
  }
  updateModelListView(names);
  setListSiteId(ids);
}

void ChangeSite::setListSiteId(QList<int> listSiteId) {
  if (mListSiteId.size() != 0) mListSiteId.clear();
  this->mListSiteId = listSiteId;
}

void ChangeSite::stopSearch() {
  if (mSearchTimer->isActive()) {
    disconnect(mSearchTimer, &QTimer::timeout, this, &ChangeSite::onSearch);
    mSearchTimer->stop();
  }
}

void ChangeSite::onSearch() {
  mCurrentKeyword = mSearchLineEdit->text();
  mIsSearching = true;
  QStringList tokens = mCurrentKeyword.trimmed().split(" ");
  for (int i = 0; i < mListSiteName.size(); ++i) {
    QString siteName = mListSiteName.at(i);
    QString siteNameSlower = siteName.toLower();
    for (int index = 0; index < tokens.size(); ++index) {
      QString token = tokens.at(index);
      if (token.contains(siteNameSlower)) {
        mListSearch.append(mListSiteName.at(i));
        break;
      }
    }
  }
  mSiteModel->setStringList(mListSearch);
}
