#ifndef APPLISTWIDGET_H
#define APPLISTWIDGET_H

#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class AppListWidget : public QWidget {
  Q_OBJECT
 public:
  AppListWidget(QWidget *parent = 0);
  void initSelectionAppNameWidget();
  QStringList mListAppName;
  QWidget *mSelectionWidget;
  QVBoxLayout *mSelectionLayout;

  QWidget *mSearchWidget;
  QVBoxLayout *mSearchLayout;
  QLineEdit *mSearchLineEdit;
  QListView *mAppNameListView;
  QStringListModel *mAppNameModel;
  QStringList *mListStringAppNameName;
  QVBoxLayout *mainVBoxLayout;

  void updateModelListView(QStringList listAppName);

  QList<int> mListAppNameId;
  QStringList mListSearch;
 Q_SIGNALS:
  void selectApp(QString appName);
 private Q_SLOTS:
  void valueChangedAppName();
};

#endif  // APPLISTWIDGET_H
