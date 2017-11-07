#ifndef CHANGESITE_H
#define CHANGESITE_H

#include <Site/sitechild.h>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
class ChangeSite : public QWidget {
  Q_OBJECT
 public:
  ChangeSite(QWidget *parent = 0);
  SiteChild *childSite = Q_NULLPTR;
  void initSelectionSiteWidget();
  QWidget *mSelectionWidget;
  QVBoxLayout *mSelectionLayout;

  QWidget *mSearchWidget;
  QVBoxLayout *mSearchLayout;
  QLineEdit *mSearchLineEdit;
  QListView *mSiteListView;
  QStringListModel *mSiteModel;
  QStringList *mListStringSiteName;
  QVBoxLayout *mainVBoxLayout;
  void updateModelListView(QStringList listName);
  void setListSites(SiteChild *childSite);
  bool mIsSearching = false;
  QHash<int, QString> mCurrentOriginalHash;
  QHash<int, QString> mCurrentMinimalHash;

  QHash<int, QString> mFilteredOriginalHash;
  QHash<int, QString> mFilteredMinimalHash;
  int mMaxLengthKeyword = 0;
  QTimer *mSearchTimer;
  QString mLastKeyword = "";
  QString mCurrentKeyword = " ";
  QStringList mListSiteName;
  QList<int> mListSiteId;
  QStringList mListSearch;
  void stopSearch();
  void startSearch();
  void setChildSite(SiteChild *childSite);
  void setListSiteId(QList<int>);
 Q_SIGNALS:
  void selectSite(QString siteName, int id, int row);
 private Q_SLOTS:
  void onSearch();
  void valueChangedSite();
};

#endif  // CHANGESITE_H
