#ifndef SEEKBARWIDGET_H
#define SEEKBARWIDGET_H

#include <math.h>
#include <QDateTime>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRectF>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include <QList>
#include "Common/generaldefine.h"

class SeekBarWidget : public QWidget {
    Q_OBJECT
private:
    QList<int> listStateOfHour;
    int leftPadding = 30;
    int rightPadding = 30;
    int bottomPadding = 20;

    int textWidth = 80;
    int textHeight = 20;
    float realWidth = 0; //width của seek bar không tính padding left và right
    float realHeight = 0;
    float mapX = 0;

    long startTimestamp = 0;
    int timeDuration = 3600;

    int totalDuration = 3600;

    long endTimestamp = 0;
    long currentTimestamp = 0;
    float currentPercent = 0.0;
    int currentValue = 0;


    float timeScale = 0.0;
    long pressTimestamp = 0;
    long releaseTimestamp = 0;

    QString startTimeString = "00:00:00";
    QString endTimeString = "00:00:00";
    QString currentTimeString = "00:00:00";

    float pointerX = leftPadding;

    QTimer *pointerTimer;

    void handleMousePositon(QPointF pos);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void drawText(QPainter &painter);
    void drawPointer(QPainter &painter);
    void drawBar(QPainter &painter);
    void drawMap(QPainter &painter);
    void drawBackground(QPainter &painter);

public:
    void startMovePointer();
    void stopMovePointer();
//    void drawDataMapFrom(int startTimestamp, int timeDuration);
    void drawDataMapFrom(QList<int> listMapOfHour);

    SeekBarWidget(QWidget *parent = 0);

    int getStartTimestamp() const;
    void setStartTimestamp(int value);

    int getTimeDuration() const;
    void setTimeDuration(int value);

    QList<int> getListStateOfHour() const;

    void setListStateOfHour(const QList<int> &value);
    void updateTimeStamp(long timestamp, int duration);
public Q_SLOTS:
    void onMovePointer();

Q_SIGNALS:
    void seekNewPositon(long newPositon, int duration);
    void updateTimeRangeCurrent(TimeRange timeRangeCurrent);
};

#endif  // SEEKBARWIDGET_H
