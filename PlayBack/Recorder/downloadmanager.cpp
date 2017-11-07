#include "downloadmanager.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
DownloadManager::DownloadManager(QObject *parent) : QObject(parent) {
  //    connect(&manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadFinished(QNetworkReply*)));
}
void DownloadManager::doDownload(const QUrl &url, QString filePath) {
  mFilePath = filePath;
  //    qDebug()<<"doDownLoad";
  mStarted = false;
  QNetworkRequest request(url);
  reply = manager.get(request);
  currentDownloads.append(reply);
  connect(reply, &QNetworkReply::downloadProgress, this,
          &DownloadManager::processbarDownload);
  connect(reply, &QNetworkReply::finished, this,
          &DownloadManager::downloadFinished);
  connect(reply, &QNetworkReply::readyRead, this,
          &DownloadManager::downloadPackage);
}

bool DownloadManager::saveToDisk(QUrl url, const QString &filename,
                                 QWidget *parent) {
  QString fileName =
      QFileDialog::getSaveFileName(Q_NULLPTR, tr("Lưu video"), filename,
                                   tr("Video File (*.mp4);;All Files (*)"));
  qDebug() << Q_FUNC_INFO << fileName;
  if (fileName.isEmpty()) {
    qDebug() << "cancel save";
    Q_EMIT cancelSaveVideoRecord();
    return false;
  } else {
    doDownload(url, fileName);
    return true;
  }
}

QString DownloadManager::saveFileName(const QUrl &url) {
  QString path = url.path();
  QString basename = QFileInfo(path).fileName();

  if (basename.isEmpty()) basename = "download";

  if (QFile::exists(basename)) {
    // already exists, don't overwrite
    int i = 0;
    basename += '.';
    while (QFile::exists(basename + QString::number(i))) ++i;
    basename += QString::number(i);
  }

  return basename;
}

void DownloadManager::downloadPackage() {
  qDebug() << "b" << reply->pos() << reply->bytesAvailable();
  QByteArray data = reply->readAll();
  qDebug() << "\tdata" << data.size();
  qDebug() << "a" << reply->pos() << reply->bytesAvailable();
  QFile file(mFilePath);
  file.open(QIODevice::ReadOnly | QIODevice::Append);
  file.seek(file.size());
  file.write(data);
  file.close();
}

void DownloadManager::downloadFinished() {
  qDebug() << reply->pos() << reply->bytesAvailable();
  QUrl url = reply->url();
  if (reply->error()) {
    Q_EMIT linkRecordNotFound();
    fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(),
            qPrintable(reply->errorString()));
  } else {
    Q_EMIT downloadVideoSuccess();
  }

  currentDownloads.removeAll(reply);
  reply->deleteLater();
  deleteLater();
}

void DownloadManager::startDownloading(QUrl url, QWidget *parent) {
  QString filename = saveFileName(url);
  saveToDisk(url, filename, parent);
}

void DownloadManager::processbarDownload(qint64 bytesReceived,
                                         qint64 bytesTotal) {
  //    qDebug()<<"total bytes"<<bytesTotal <<"recevied"<<bytesReceived;
  if (bytesTotal != 0) {
    double percent = (bytesReceived * 1.0) / bytesTotal;
    int percentInt = percent * 100;
    Q_EMIT percentProcessbar(percentInt);
  }
}
