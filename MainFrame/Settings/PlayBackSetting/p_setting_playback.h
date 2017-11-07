#ifndef P_SETTINGPLAYBACK_H
#define P_SETTINGPLAYBACK_H

#include <PacModel/presentation.h>
#include <QAction>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QMenu>
#include <QMimeDatabase>
#include <QObject>
#include <QProgressDialog>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "../c_settings.h"
#include "c_setting_playback.h"
#include <QSettings>
class C_SettingPlayBack;
class C_Setting;
class P_SettingPlayBack : public Presentation {
  //  Q_OBJECT
 private:
  QWidget *zone;
 public:
  P_SettingPlayBack(Control *ctrl, QWidget *zone);
  void update();
  C_SettingPlayBack *control() { return (C_SettingPlayBack *)ctrl; }
  QObject *getZone(int zoneId);

 private Q_SLOTS:
  void browse();
  void animateFindClick();

 private:
  QComboBox *createComboBox(const QString &text = QString());
  QComboBox *fileComboBox;
  QComboBox *textComboBox;
  QComboBox *directoryComboBox;
  QLabel *filesFoundLabel;
  QPushButton *findButton;
  QTableWidget *filesTable;

  QDir currentDir;
};

#endif  // P_SETTINGPLAYBACK_H
