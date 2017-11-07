#ifndef LOGUTILS_H
#define LOGUTILS_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QString>
#include <QTextStream>

class LogUtils {
 public:
  static LogUtils &instance() {
    static LogUtils mInstance;
    return mInstance;
  }

  LogUtils(LogUtils const &) = delete;
  LogUtils(LogUtils &&) = delete;
  LogUtils &operator=(LogUtils const &) = delete;
  LogUtils &operator=(LogUtils &&) = delete;

  static const QString K_LOG_ROOT;

  void logPerfomance(QString);

  void setWrittenToFile(bool);
  void setWrittenToConsole(bool);

 private:
  QString mLogFolder;

  QString mLogFileName = "log_";
  QString mDiaryFileName = "diary_";

  QFile *mLogFile;
  QFile *mDiaryFile;

  bool isWrittenToFile = false;
  bool isWrittenToConsole = false;

 private:
  LogUtils();
  ~LogUtils();
};

#endif  // LOGUTILS_H
