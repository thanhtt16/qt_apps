#ifndef UTILITIES_H
#define UTILITIES_H

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QProcess>
#include <QString>
#include "logutils.h"

struct TestDiary {
  int pid;

  QElapsedTimer timer;
  QDateTime startDateTime;
  QDateTime currentDateTime;
  qint64 startTime;
  qint64 currentTime;
  qint64 timeElapsed;
  int hoursElapsed;
  int minutesElapsed;
  int secondsElapsed;

  int wallLayout;
  int numberOfStream;
  QString streamType;

  int threads = 4;
  int buffer = 1024;
  int cpuMHz;
  int ramMiB;

  float usageCpu;
  float usageRam;

  int decodeFailureCount;
  int lostPackageCount;
  int playStableCount;

  void getSystemInfo() {
    pid = QCoreApplication::applicationPid();
    timer.start();
    startDateTime = QDateTime::currentDateTime();
    startTime = startDateTime.currentMSecsSinceEpoch();
    QProcess process;
    process.start("lshw");
    process.waitForFinished();
    QString data = process.readAllStandardOutput();
    process.close();
    QString search = "size: ";
    int indexOfRam = data.indexOf(search) + search.length();
    QString ramString = data.mid(indexOfRam, 8)
                            .replace(" ", "")
                            .replace("M", "")
                            .replace("i", "")
                            .replace("B", "")
                            .replace("\n", "");
    int indexOfCpu = data.indexOf(search, indexOfRam) + search.length();
    QString cpuString = data.mid(indexOfCpu, 4);
    cpuMHz = cpuString.toInt();
    ramMiB = ramString.toInt();
  }

  void setNumberOfStream(int number) { numberOfStream = number; }

  void setWallLayout(int size) { wallLayout = size; }

  void setType(QString type) { streamType = type; }

  void getUsage() {
    QProcess process;
    QString command =
        QString("ps --no-header -p %1 -o pcpu,pmem,rss,vsz").arg(pid);
    process.start(command);
    process.waitForFinished();
    QString data = process.readAllStandardOutput().trimmed();
    process.close();
    QStringList datas = data.replace("  ", " ").split(" ");
    if (datas.size() == 4) {
      usageCpu = ((QString)datas.at(0)).toFloat() / threads;
      usageRam = ((QString)datas.at(2)).toFloat() / buffer;
    }
    currentDateTime = QDateTime::currentDateTime();
    currentTime = currentDateTime.currentMSecsSinceEpoch();
    timeElapsed = (currentTime - startTime) / 1000;
    qint64 temp = timeElapsed;
    hoursElapsed = temp / 3600;
    temp -= hoursElapsed * 3600;
    minutesElapsed = temp / 60;
    temp -= minutesElapsed * 60;
    secondsElapsed = temp;
  }

  void showUsage() {
    //    QString dateTimeFormat = "dd/MM/yyyy hh:mm:ss";
    //    LogUtils::instance().logPerfomance(text);
  }
};
#endif  // UTILITIES_H
