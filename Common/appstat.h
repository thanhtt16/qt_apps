#ifndef UTILITIES_H
#define UTILITIES_H

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QNetworkInterface>
#include <QProcess>
#include <QString>
#include "logutils.h"
static struct AppStat {
  int pid;

  QString dateTimeFormat = "dd/MM/yyyy hh:mm:ss";
  QDateTime startDateTime;
  QDateTime currentDateTime;
  QString startDateTimeString;
  QString currentDateTimeString;
  qint64 startTime;
  qint64 currentTime;
  qint64 timeElapsed;
  qint64 dayElapsed;
  QString timeElapsedString;
  int hoursElapsed;
  int minutesElapsed;
  int secondsElapsed;
  bool isNight = false;

  int wallLayout;
  int numberOfStream;
  QString streamType;

  int threads = 4;
  int buffer = 1024;
  int cpuMHz;
  int ramMiB;

  float usageCpu;
  float usageRam;

  int videoPausingCount;
  int videoStoppedCount;
  int videoPlayingCount;
  QString os_UpTime;
  QString app_Uptime;
  QString cpu_Temp_Average;  // nhiet do
  float cpu_temp_crit;
  float cpu_temp_high;
  QString ipAddressDevice;
  QString nameDevice;
  QString getIpDevice() {
    QProcess process;
    process.start("curl https://api.ipify.org/?format=json");
    process.waitForFinished();
    ipAddressDevice = QString(process.readAllStandardOutput());
    if (!ipAddressDevice.isEmpty()) {
      int index = ipAddressDevice.indexOf(":");
      if (index > 0) {
        ipAddressDevice =
            ipAddressDevice.replace("\"}", "").replace("{\"ip\":\"", "");
      }
    }
    process.close();
    return ipAddressDevice;
  }
  QString getNameDevice() {
    QProcess process;
    process.start("hostname");
    process.waitForFinished();
    nameDevice = QString(process.readAllStandardOutput());
    nameDevice = nameDevice.replace("\n", "");
    process.close();
    return nameDevice;
  }
  qint64 getTimLapse() { return timeElapsed; }
  int getTotalRam() { return ramMiB; }
  int getTotalCpu() { return cpuMHz; }
  float getUsageCpu() { return usageCpu; }
  float getUsageRam() { return usageRam; }
  void getSystemInfo() {
    getStartDateTime();
    updateCurrentDateTime();

    pid = QCoreApplication::applicationPid();
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
  QString getTimeUpOS() {
    QProcess process;
    process.start("uptime");
    process.waitForFinished();
    os_UpTime = QString(process.readAllStandardOutput());
    if (!os_UpTime.isEmpty()) {
      QStringList list = os_UpTime.split(",");
      if (!list.isEmpty()) {
        os_UpTime = os_UpTime.split(",").first();
        QStringList list1 = os_UpTime.split("up");
        if (!list1.isEmpty()) {
          os_UpTime = list1.last().trimmed();
        }
      }
    }
    process.close();
    return os_UpTime;
  }
  QString getTimeUpApp() {
    QProcess process;
    process.start(
        "ps xua | grep AppImage | grep -v grep | grep -v '0:00' | awk '{print "
        "$10/60}'");
    process.waitForFinished();
    app_Uptime = QString(process.readAllStandardOutput());
    process.close();
    return app_Uptime;
  }
  QString getCpuTempAverage() {
    QProcess process;
    process.start(
        "cat /sys/class/thermal/thermal_zone0/temp | awk '{print $1}'");
    process.waitForFinished();
    cpu_Temp_Average = QString(process.readAllStandardOutput());
    float cpu_Temp_Average_Temp = cpu_Temp_Average.toFloat() / 1000;
    cpu_Temp_Average = QString::number(cpu_Temp_Average_Temp);
    process.close();
    return cpu_Temp_Average;
  }
  float getCpuTempCrit() {
    QProcess process;
    process.start(
        "cat /sys/bus/platform/devices/coretemp.0/hwmon/hwmon0/temp1_crit | "
        "awk '{print $1}'");
    process.waitForFinished();
    QString cpu_Temp_Crit_Temp_String =
        QString(process.readAllStandardOutput());
    float cpu_temp_crit = cpu_Temp_Crit_Temp_String.toFloat() / 1000;
    process.close();
    return cpu_temp_crit;
  }
  float getCpuTempHigh() {
    QProcess process;
    process.start(
        "cat /sys/bus/platform/devices/coretemp.0/hwmon/hwmon0/temp3_max | awk "
        "'{print $1}'");
    process.waitForFinished();
    QString cpu_temp_high_Temp_String =
        QString(process.readAllStandardOutput());
    float cpu_temp_high = cpu_temp_high_Temp_String.toFloat() / 1000;
    process.close();
    return cpu_temp_high;
  }

  void setNumberOfStream(int number) { numberOfStream = number; }

  void setWallLayout(int size) { wallLayout = size; }

  void setType(QString type) { streamType = type; }

  void updateUsage() {
    updateCurrentDateTime();
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
  }

  void getStartDateTime() {
    startDateTime = QDateTime::currentDateTime();
    startTime = startDateTime.currentMSecsSinceEpoch();
    startDateTimeString = startDateTime.toString(dateTimeFormat);
  }

  void updateCurrentDateTime() {
    currentDateTime = QDateTime::currentDateTime();
    currentDateTimeString = currentDateTime.toString(dateTimeFormat);
    currentTime = currentDateTime.currentMSecsSinceEpoch();
    updateElapsedTime();
    int hour = currentDateTime.time().hour();
    if (hour >= 6 && hour <= 18) {
      isNight = false;
    } else {
      isNight = true;
    }
  }

  void updateElapsedTime() {
    timeElapsed = (currentTime - startTime - 1) / 1000;
    qint64 temp = timeElapsed;
    dayElapsed = temp / 86400;
    temp -= dayElapsed * 86400;
    hoursElapsed = temp / 3600;
    temp -= hoursElapsed * 3600;
    minutesElapsed = temp / 60;
    temp -= minutesElapsed * 60;
    secondsElapsed = temp;
    timeElapsedString = QString("%1 days %2:%3:%4")
                            .arg(dayElapsed, 2, 'd', 0, '0')
                            .arg(hoursElapsed, 2, 'd', 0, '0')
                            .arg(minutesElapsed, 2, 'd', 0, '0')
                            .arg(secondsElapsed, 2, 'd', 0, '0');
  }

  void showUsage() {}
} appStat;
#endif  // UTILITIES_H
