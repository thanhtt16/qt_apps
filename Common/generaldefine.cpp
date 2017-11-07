#include "generaldefine.h"

QString GeneralDefine::convertDate(long unixTime) {
  QDateTime timestamp;
  timestamp.setTime_t(unixTime);
  return timestamp.toString("dd/MM/yyyy");
}

QString GeneralDefine::checkDay(QString date) {
  QString dateToday = QDateTime::currentDateTime().toString("dd/MM/yyyy");
  long unixtimeToDay = QDateTime::currentDateTime().toTime_t();
  long unixtimeYesterday = (unixtimeToDay - 3600 * 24);
  long unixtimeDayBeforeYesterday = (unixtimeYesterday - 3600 * 24);
  QString dateYesterday = convertDate(unixtimeYesterday);
  QString dateBeforeyesterday = convertDate(unixtimeDayBeforeYesterday);
  if (date.compare(dateToday) == 0) {
    return "Hôm nay";
  } else if (date.compare(dateYesterday) == 0) {
    return "Hôm qua";
  } else if (date.compare(dateBeforeyesterday) == 0) {
    return "Hôm kia";
  } else {
    return date;
  }
}
TypeTime GeneralDefine::convertSecondsToTexts(long seconds) {
  TypeTime resultTime;
  QDateTime timestamp;
  timestamp.setTime_t(seconds);
  QString timeString = timestamp.toString("dd/MM/yyyy hh:mm");
  QStringList list1 = timeString.split(" ");
  resultTime.day = list1[0];
  resultTime.time = list1[1];
  return resultTime;
}

long GeneralDefine::convertDateToTimeStamp(QString date, QString hour,
                                           QString minute, QString second) {
  QDateTime timeConvertor;
  if (hour.length() < 2) {
    QString tmpHour = "0";
    hour = tmpHour.append(hour);
  }
  if (minute.length() < 2) {
    QString tmpMinute = "0";
    minute = tmpMinute.append(minute);
  }
  if (second.length() < 2) {
    QString tmpSecond = "0";
    second = tmpSecond.append(second);
  }

  QString timeString =
      hour.append(":").append(minute).append(":").append(second);
  QString dateTimeConvert = date.append(" ").append(timeString);
  timeConvertor = QDateTime::fromString(dateTimeConvert, "dd-MM-yyyy HH:mm:ss");
  int timestamp = timeConvertor.toTime_t();
  return timestamp;
}
GeneralDefine::GeneralDefine() {}

GeneralDefine::~GeneralDefine() {}
