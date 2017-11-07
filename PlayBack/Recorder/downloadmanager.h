#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <stdio.h>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QWidget>

class TypeTime;
class DownloadManager : public QObject {
  Q_OBJECT
  QNetworkAccessManager manager;
  QList<QNetworkReply *> currentDownloads;
  QNetworkReply *reply;
  bool mStarted = false;
  QString mFilePath;

 public:
  DownloadManager(QObject *parent);
  void doDownload(const QUrl &url, QString filePath);
  QString saveFileName(const QUrl &url);
  bool saveToDisk(QUrl url, const QString &filename, QWidget *parent);
  void startDownloading(QUrl url, QWidget *parent);
 public Q_SLOTS:
  //    void execute();
  void downloadFinished();
  void downloadPackage();
  void processbarDownload(qint64 bytesReceived, qint64 bytesTotal);
 Q_SIGNALS:
  void saveFileVideoVODSuccess(QString);
  void downloadVideoSuccess();
  void cancelSaveVideoRecord();
  void percentProcessbar(qint64 percent);
  void linkRecordNotFound();
};

#endif  // DOWNLOADMANAGER_H
