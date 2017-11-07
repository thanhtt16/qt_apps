#ifndef MS_PROFILEGRIDWIDGET_H
#define MS_PROFILEGRIDWIDGET_H

#include <QComboBox>
#include <QGridLayout>
#include <QHash>
#include <QLineEdit>
#include <QList>
#include <QListView>
#include <QMap>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QSpinBox>
#include <QStringList>
#include <QStringListModel>
#include <QWidget>
#include "Camera/camitem.h"
#include "Camera/camsite.h"
#include "Common/utilities.h"
#include "MagicShow/ms_profilebook.h"
#include "MagicShow/ms_profilecellwidget.h"
#include "MagicShow/ms_profilepage.h"
#include "math.h"

class ProfileGridWidget : public QWidget {
  Q_OBJECT
 public:
  ProfileGridWidget(QWidget *parent);
  void setCamSite(CamSite *);

  void initSelectionWidget();
  void showSelectionWidget(int);
  void hideSelectionWidget();
  void updateSelectionWidgetPosition();

  void initProfileBook(int, int, int);
  ProfileBook *getCurrentProfileBook();
  void setProfileBook(ProfileBook *, int);

  int getCurrentPage();
  int getTotalPage();
  int getCurrentLayout();
  void addPage();
  void updateCurrentPageData();
  void updatePageButtons();
  void updateCurrentPageLayout();

  void startAnimation(bool);
  void updateCurrentPageLayoutButtons();
  void updatePageWidgetSize();

 private:
  ProfileBook *mProfileBook;
  ProfilePage *mProfilePage;
  CamSite *mCamSite;

  //        QHash<int, QString> mOriginalData;
  //        QHash<int, QString> mMinimalData;
  //        QHash<int, QString> mSelectedData;
  //        QHash<int, QString> mFilteredData;

  QHash<int, QString> mOriginalHash;
  QHash<int, QString> mMinimalHash;

  QHash<int, QString> mCurrentOriginalHash;
  QHash<int, QString> mCurrentMinimalHash;

  QHash<int, QString> mFilteredOriginalHash;
  QHash<int, QString> mFilteredMinimalHash;

  QHash<int, QString> mSelectedOriginalHash;
  QHash<int, QString> mSelectedMinimalHash;

  QPushButton *createPageButton();

  QList<ProfileCellWidget *> mProfileCellWidgets;
  QList<ProfileCellWidget *> mPoolCellWidgets;
  bool mIsAnimating = false;
  bool mIsOpen = true;
  bool mIsSearching = false;
  QHBoxLayout *mMainLayout;
  QVBoxLayout *mSubLayout;
  QVBoxLayout *mLeftLayout;
  QVBoxLayout *mRightLayout;
  QHBoxLayout *mBotLayout;

  QList<QPushButton *> mLayoutButtons;
  QPushButton *mClearButton;
  QPushButton *mApplyButton;
  QLineEdit *mNameLineEdit;
  QComboBox *mTransitionComboBox;
  QSpinBox *mDurationSpinBox;

  QLabel *mPageLabel;
  QPushButton *mAddPageButton;
  QPushButton *mDelPageButton;
  QWidget *mPageWidget;
  QVBoxLayout *mPageLayout;
  QScrollArea *mPageScrollArea;
  QList<QPushButton *> mPageButtons;

  QWidget *mLeftWidget;
  QWidget *mBotWidget;
  QWidget *mRightWidget;
  QWidget *mTopWidget;

  QGridLayout *mGridLayout;

  int mLeftWidth = 80;
  int mBotHeight = 50;

  int mMaxLayout = 0;
  int mNumberOfLayout = sqrt(mMaxLayout) - 1;
  int mDefaultLayout = 0;

  int mMaxPage = 100;
  int mCurrentLayout = 0;

  int mLastLayout = 0;
  int mCurrentSize = 0;
  int mCurrentPage = 0;
  int mTotalPage = 0;
  int mMaxLengthKeyword = 0;
  int mCurrentProfileCellIndex = 0;
  void initMainUI();
  void initLeftSide();
  void initRightSide();
  void initTopSide();
  void initBotSide();
  void initUI();

  QWidget *mSelectionWidget;
  QLabel *mSelectionLabel;
  QLineEdit *mSearchLineEdit;
  QListView *mCamPositionListView;
  QStringListModel *mCamPositions;
  QVBoxLayout *mSelectionLayout;
  QPushButton *mSelectButton;
  QPushButton *mCancelButton;

  QTimer *mSearchTimer;
  QString mLastKeyword = "";
  QString mCurrentKeyword = " ";
  void startSearch();
  void stopSearch();

  bool mIsCreateNew = true;
  int mProfileBookIndex = 0;

 protected:
  void resizeEvent(QResizeEvent *);
 Q_SIGNALS:
  void createdProfileBook(ProfileBook *);
  void updatedProfileBook(ProfileBook *);
  void cancelProfileBook();
 public Q_SLOTS:
  void onAnimationEnd();
  void filterWithKeyword(QString);
  void onSearch();
  void onCancel();
  void onSelect();
  void onDeleteAt(int);
  void onApplyButtonClicked();
  void onLayoutButtonClicked();
  void onAddPageButtonClicked();
  void onDelPageButtonClicked();
  void onPageButtonClicked();
  void onClearButtonClicked();
  void displayCurrentPage();
  void onTransitionComboBoxSelectedIndex(int index);
  void onDurationSpinBoxValueChanged(int value);
};

#endif  // PROFILEGRIDWIDGET_H
