#ifndef APPSETTINGWIDGET_H
#define APPSETTINGWIDGET_H

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QSpinBox>
#include <QStringList>
#include <QStringListModel>
#include <QTabBar>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWidget>

#include "appconfig.h"
#include "appprofile.h"
#include "resources.h"

class AppSettingWidget : public QWidget {
  Q_OBJECT
 private:
  QHBoxLayout *pMainLayout;

  QWidget *pLeftWidget;
  QVBoxLayout *pLeftLayout;

  QWidget *pLeftTopWidget;
  QHBoxLayout *pLeftTopLayout;
  QLineEdit *pNameLineEdit;
  QPushButton *pAddButton;

  QListView *pNameListView;
  QStringListModel *pNameListModel;

  QWidget *pBotLeftWidget;
  QHBoxLayout *pBotLeftLayout;
  QPushButton *pApplyButton;
  QPushButton *pDeleteButton;
  void initLeftSide();

  QWidget *pRightWidget;
  QVBoxLayout *pRightLayout;
  //    QTreeWidget *pConfigTreeWidget;
  //    QTreeWidgetItem *pConfigTreeItem;

  QWidget *pRightTopWidget;
  QVBoxLayout *pRightTopLayout;
  QTabWidget *pConfigTabWidget;

  QLabel *createLabel(QWidget *parent, QString text);
  QLineEdit *createLineEdit(QWidget *parent, QString text);
  QSpinBox *createSpinBox(QWidget *parent, int minValue, int maxValue,
                          int stepValue, int initValue, QString suffix);

  QWidget *pNetworkWidget;
  QGridLayout *pNetworkLayout;

  QLabel *pPingHostLabel;
  QLabel *pBaseApiUrlLabel;
  QLabel *pBaseRecordApiUrlLabel;
  QLabel *pBaseVodApiUrlLabel;
  QLabel *pTokenUriLabel;
  QLabel *pLoginUriLabel;
  QLabel *pRecordUriLabel;
  QLabel *pCamSiteByDeviceUriLabel;
  QLabel *pCamSiteBySiteUriLabel;

  QLineEdit *pPingHostLineEdit;
  QLineEdit *pBaseApiUrlLineEdit;
  QLineEdit *pBaseRecordApiUrlLineEdit;
  QLineEdit *pBaseVodApiUrlLineEdit;
  QLineEdit *pTokenUriLineEdit;
  QLineEdit *pLoginUriLineEdit;
  QLineEdit *pRecordUriLineEdit;
  QLineEdit *pCamSiteByDeviceUriLineEdit;
  QLineEdit *pCamSiteBySiteUriLineEdit;
  void initNetworkWidget();

  QWidget *pLayoutWidget;
  QGridLayout *pLayoutLayout;

  QLabel *pMaxLayoutLabel;
  QLabel *pSlowFpsLabel;
  QLabel *pStopFpsLabel;
  QLabel *pWaitReplaySecondsLabel;

  QSpinBox *pMaxLayoutSpinBox;
  QSpinBox *pSlowFpsSpinBox;
  QSpinBox *pStopFpsSpinBox;
  QSpinBox *pWaitReplaySecondsSpinBox;

  void initLayoutWidget();

  QWidget *pRightBotWidget;
  QHBoxLayout *pRightBotLayout;
  QPushButton *pSaveButton;
  QPushButton *pCancelButton;

  void initRightSide();
  void initRightTopSide();
  void initRightBottoomSide();

  void initUI();

  AppProfile *pCurrentProfile;
  void displayCurrentProfile();
  int mLastSelectedIndex = -1;
  int mCurrentSelectedIndex = -1;
  bool mIsChanged = false;

 public:
  AppSettingWidget(QWidget *parent = 0);
  ~AppSettingWidget();

 public Q_SLOTS:
  void onAddClicked();
  void onApplyClicked();
  void onDeleteClicked();
  void onSaveButtonClicked();
  void onCancelButtonClicked();
  void onSelectedProfileAt(QModelIndex modelIndex);
};

#endif  // APPSETTINGWIDGET_H
