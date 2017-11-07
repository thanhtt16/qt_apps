#include "datamapwidget.h"

DataMapWidget::DataMapWidget(QWidget *parent) : QWidget(parent) {
  setFixedHeight(50);
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &DataMapWidget::onTimeout);
  image = QImage(":/images/res/marker.png");
  qDebug() << image.width() << image.height();
}

void DataMapWidget::startTimer() {
  if (!timer->isActive()) {
    timer->start(1000);
  }
}

void DataMapWidget::stopTimer() {
  if (timer->isActive()) {
    timer->stop();
  }
}

void DataMapWidget::loadDataMap(QString url, long startTime,
                                long timeDuration) {
  stopTimer();
  QEventLoop eventLoop;
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop,
                   SLOT(quit()));
  QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
  eventLoop.exec();
  if (reply->error() == QNetworkReply::NoError) {
    QString json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    QStringList keys = jsonObject.keys();
    MyLessThan le;
    qSort(keys.begin(), keys.end(), le);
    dataMaps.clear();
    dataItems.clear();
    for (int index = 0; index < keys.size(); ++index) {
      QString key = keys.at(index);
      QJsonArray jsonArray = jsonObject.value(key).toArray();
      DataMap dataMap;
      dataMap.setKey(key);
      dataMap.parse(jsonArray);
      dataMaps.append(dataMap);
      dataItems.append(dataMap.dataItems);
    }
    this->parseDataMap(startTime, timeDuration);
    this->runningTime = startTime;
    this->update();
    this->startTimer();
    delete reply;
  } else {
    this->startTime = startTime;
    this->timeDuration = timeDuration;
    this->runningTime = startTime;
    this->update();
    this->startTimer();
    delete reply;
  }
}

void DataMapWidget::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event)
  movePoint.setX(xPercent * this->width() / 100);
}

void DataMapWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter;
  painter.begin(this);
  painter.fillRect(QRect(0, 0, this->width(), this->height()),
                   QColor("#8e8e93"));
  if (!dataMaps.isEmpty()) {
    drawDataMap(painter);
  }
  QPen penText(QColor("#ffffff"), 10, Qt::DotLine, Qt::FlatCap, Qt::RoundJoin);
  painter.setPen(penText);
  painter.drawImage(movePoint.x() - image.width() / 2, this->height() - 20,
                    image);
  QString date =
      QDateTime::fromMSecsSinceEpoch(runningTime).toString("hh:mm:ss");
  painter.fillRect(movePoint.x() - 35, this->height() - 37, 70, 25,
                   QColor("#1e1e1e"));
  painter.drawText(movePoint.x() - 30, this->height() - 20, date);
  painter.fillRect(QRect(0, this->height() - 5, this->width(), 5),
                   QColor("#edf0f5"));
  painter.fillRect(QRect(0, this->height() - 5, movePoint.x(), 5),
                   QColor("#1e61ee"));
  painter.end();
}

void DataMapWidget::parseDataMap(long startTime, long length) {
  this->startTime = startTime;
  this->timeDuration = length;
  this->endTime = this->startTime + this->timeDuration;
  this->startIndex = 0;
  this->endIndex = 0;
  for (int iStartIndex = 0; iStartIndex < dataItems.size(); ++iStartIndex) {
    DataItem dataItem = dataItems.at(iStartIndex);
    if (dataItem.start <= startTime && dataItem.end >= startTime) {
      this->startIndex = iStartIndex;
      //      qDebug() << "startIndex" << startIndex;
      //      dataItem.print();
      for (int iEndIndex = this->startIndex + 1; iEndIndex < dataItems.size();
           ++iEndIndex) {
        DataItem dataItem = dataItems.at(iEndIndex);
        if (dataItem.start <= endTime && dataItem.end >= endTime) {
          this->endIndex = iEndIndex;
          //          qDebug() << "endIndex" << iEndIndex;
          //          dataItem.print();
          break;
        }
      }
      break;
    }
  }
}

void DataMapWidget::drawDataMap(QPainter &painter) {
  xPos = 0.0;
  for (int index = startIndex; index <= endIndex; ++index) {
    DataItem dataItem = dataItems.at(index);
    long duration = 0;
    if (index == startIndex) {
      duration = dataItem.end - startTime;
    } else if (index == endIndex) {
      duration = endTime - dataItem.start;
    } else {
      duration = dataItem.duration;
    }
    double width =
        (double)duration / (double)this->timeDuration * this->width();
    painter.fillRect(QRect(xPos, 0, width, this->height()),
                     dataItem.data ? QColor("#58eaa1") : QColor("#ef456a"));
    xPos += width;
    //    QString log = QString("%1 %2 %3 %4")
    //                      .arg(index, 5)
    //                      .arg(duration, 10)
    //                      .arg(width, 10, 'f', 2)
    //                      .arg(dataItem.data, 5);
    //    qDebug() << log;
    //    dataItem.print();
  }
}

void DataMapWidget::mousePressEvent(QMouseEvent *event) {
  isMoving = true;
  moveMarkerTo(event->pos());
  long timestamp = movePoint.x() / this->width() * timeDuration / 1000;
  Q_EMIT onPress(timestamp);
}

void DataMapWidget::mouseMoveEvent(QMouseEvent *event) {
  if (isMoving) {
    moveMarkerTo(event->pos());
  }
}

void DataMapWidget::moveMarkerTo(QPointF position) {
  movePoint = position;
  if (movePoint.x() < 0) {
    movePoint.setX(0);
  } else if (movePoint.x() > this->width()) {
    movePoint.setX(this->width());
  }
  xPercent = (double)movePoint.x() / (double)this->width() * 100.0;
  if (xPercent < 0) {
    xPercent = 0;
  } else if (xPercent > 100) {
    xPercent = 100;
  }
  runningTime = startTime + xPercent * timeDuration / 100.0;
  update();
}

void DataMapWidget::onTimeout() {
  if (!isMoving) {
    double speed = (double)this->width() / (double)(timeDuration / 1000);
    if (movePoint.x() < this->width()) {
      movePoint.setX(movePoint.x() + speed);
      runningTime += 1000;
    } else {
      movePoint.setX(this->width());
    }
    update();
  }
}

void DataMapWidget::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Left: {
    double speed = (double)this->width() / (double)(timeDuration / 1000);
    movePoint.setX(movePoint.x() - speed);
    runningTime -= 1000;
    update();
  } break;
  case Qt::Key_Right: {
    double speed = (double)this->width() / (double)(timeDuration / 1000);
    movePoint.setX(movePoint.x() + speed);
    runningTime += 1000;
    update();
  } break;
  default:
    break;
  }
}

void DataMapWidget::mouseReleaseEvent(QMouseEvent *event) {
  Q_UNUSED(event)
  isMoving = false;
  long timestamp = movePoint.x() / this->width() * timeDuration / 1000;
  Q_EMIT onRelease(timestamp);
}
