#include "camstatistic.h"

QString CamStatistic::kLogFormat =
    QString()
        .append("  Bắt đầu: %1 \n")
        .append("  Chơi lại: %2 \n")
        .append("  Hiện tại: %3 \n")
        .append("  Lần chơi lại: %4 - [ %5 / %6 ] \n")
        .append("  CPU: %7 % - RAM: %8 MB \n")
        .append("  FPS: %9 - Trạng thái: %10");

CamStatistic::CamStatistic(QObject *parent) : QObject(parent) {}

QString CamStatistic::statLog(QString startDateTime, QString currentDateTime,
                              float cpu, float ram) {
  return kLogFormat.arg(startDateTime)
      .arg(mLastReconnectedDateTime)
      .arg(currentDateTime)
      .arg(mReconnectStreamCounter, 0, 'f', 0, '0')
      .arg(mDisconnectStreamCounter + 1)
      .arg(mDisconnectedStreamThreshold)
      .arg(cpu, 5, 'f', 2, '0')
      .arg(ram, 6, 'f', 2, '0')
      .arg(mCurrentFps, 5, 'f', 2, '0')
      .arg(mState);
}
