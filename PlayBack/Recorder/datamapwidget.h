#ifndef DATAMAPWIDGET_H
#define DATAMAPWIDGET_H

#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QWidget>

struct MyLessThan {
  bool operator()(const QString &s1, const QString &s2) const {
    if (s1 < s2)
      return true;
    if (s1 > s2)
      return false;
    return false;
  }
};

struct DataItem {
  int duration;
  long start;
  long end;
  bool data;
  int startHours;
  int endHours;
  double delta;

  void parse(QJsonObject jsonObject) {
    duration = jsonObject.value("duration").toInt();
    start = (long)jsonObject.value("start").toDouble();
    end = (long)jsonObject.value("end").toDouble();
    data = jsonObject.value("data").toBool();
    startHours = QDateTime::fromMSecsSinceEpoch(start).time().hour();
    endHours = QDateTime::fromMSecsSinceEpoch(end).time().hour();
    delta = round((end - start) / 3600000.0 * 10.0) / 10.0;
  }

  void print() {
    qDebug() << "\t duration:" << duration;
    qDebug() << "\t start   :" << start << startHours
             << QDateTime::fromMSecsSinceEpoch(start).toString(
                    "yyyy/MM/dd hh:mm:ss");
    qDebug() << "\t end     :" << end << endHours
             << QDateTime::fromMSecsSinceEpoch(end).toString(
                    "yyyy/MM/dd hh:mm:ss");
    qDebug() << "\t data    :" << data;
    qDebug() << "\t delta   :" << delta;
    qDebug() << "";
  }
};

struct DataMap {
  QString date;
  long timestamp;
  QList<DataItem> dataItems;
  double totalDelta = 0.0;

  void setKey(QString key) {
    QStringList keys = key.split(".");
    date = keys.first();
    timestamp = ((QString)keys.last()).toLong();
  }

  void parse(QJsonArray jsonArray) {
    for (int index = 0; index < jsonArray.count(); ++index) {
      QJsonObject jsonObject = jsonArray.at(index).toObject();
      DataItem dataItem;
      dataItem.parse(jsonObject);
      dataItems.append(dataItem);
    }
  }

  void print() {
    qDebug() << "date:" << date << "timestamp:" << timestamp
             << "count:" << dataItems.count();
    totalDelta = 0.0;
    for (int index = 0; index < dataItems.size(); ++index) {
      DataItem dataItem = dataItems.at(index);
      dataItem.print();
      totalDelta += dataItem.delta;
    }
    qDebug() << "total delta" << totalDelta;
  }
};

class DataMapWidget : public QWidget {
  Q_OBJECT
private:
  QList<DataMap> dataMaps;
  QList<DataItem> dataItems;

  int dataIndex = 0;
  long startTime = 0;
  long timeDuration = 0;
  long endTime = startTime + timeDuration;
  long runningTime = startTime;
  long lastRunningTime = 0;
  int startIndex = 0;
  int endIndex = 0;

  bool isMoving = false;
  double xPercent = 0.0;
  double xPos = 0.0;
  QPointF pressPoint;
  QPointF movePoint;

  QImage image;

  void drawDataMap(QPainter &painter);
  void moveMarkerTo(QPointF position);
  void parseDataMap(long startTime, long timeDuration);

  QTimer *timer;
  void startTimer();
  void stopTimer();

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);

public:
  explicit DataMapWidget(QWidget *parent = 0);
  void loadDataMap(QString url, long startTime, long timeDuration);

Q_SIGNALS:
  void onPress(long timestamp);
  void onRelease(long timestamp);
public Q_SLOTS:
  void onTimeout();
};

#endif // DATAMAPWIDGET_H
