#ifndef MS_VIDEOWALLPROFILECELL_H
#define MS_VIDEOWALLPROFILECELL_H

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "Common/resources.h"

class ProfileCellWidget : public QStackedWidget {
  Q_OBJECT
 public:
  ProfileCellWidget(QWidget *parent);
  void setText(QString);
  void setEnabledButton(bool);
  int getIndex();
  void setIndex(int);
  void setAdded(bool);
  bool isAdded();

 private:
  QLabel *mThumbLabel;
  QVBoxLayout *mButtonLayout;
  QPushButton *mAddDelButton;
  bool mIsAdded = false;
  int mIndex = 0;

 protected:
  void resizeEvent(QResizeEvent *);
 Q_SIGNALS:
  void showSelection(int);
  void deleteAt(int);
 public Q_SLOTS:
  void onAddDelButtonClicked();
};

#endif  // VIDEOWALLPROFILECELL_H
