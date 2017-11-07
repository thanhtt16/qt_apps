#include "logutils.h"

LogUtils::LogUtils() {
  mLogFolder = QDateTime::currentDateTime().toString("yyyy-MM-dd").append("/");
  mLogFolder = K_LOG_ROOT + mLogFolder;
  QFile logFolderFile(mLogFolder);
  if (!logFolderFile.exists()) {
    QDir dir;
    dir.mkpath(mLogFolder);
  }
  QString currentDateTime = QDateTime::currentDateTime().toString("hh-mm-ss");

  mLogFileName = mLogFolder + mLogFileName;
  mLogFileName = mLogFileName.append(currentDateTime + ".txt");
  mLogFile = new QFile(mLogFileName);
  mLogFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

  mDiaryFileName = mLogFolder + mDiaryFileName;
  mDiaryFileName = mDiaryFileName.append(currentDateTime + ".txt");
  mDiaryFile = new QFile(mDiaryFileName);
  mDiaryFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
}

LogUtils::~LogUtils() {
  mLogFile->close();
  delete mLogFile;
  mDiaryFile->close();
  delete mDiaryFile;
}

const QString LogUtils::K_LOG_ROOT = "test/done/";

void LogUtils::logPerfomance(QString text) {
  if (isWrittenToFile) {
    QTextStream(mDiaryFile) << text << "\n";
  }
}

void LogUtils::setWrittenToConsole(bool b) { isWrittenToConsole = b; }

void LogUtils::setWrittenToFile(bool b) { isWrittenToFile = b; }
