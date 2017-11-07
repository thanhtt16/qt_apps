#ifndef RTCRENDERWIDGET_H
#define RTCRENDERWIDGET_H

#include <QApplication>
#include <QBuffer>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QMutex>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QResizeEvent>
#include <QSize>
#include <QTransform>
#include <QVBoxLayout>
#include <QWheelEvent>
#include <QWidget>
#include <QtAV>
#include <QtAVWidgets>
#include "Common/generaldefine.h"

using namespace QtAV;

class RTCRenderWidget : public QObject {
    Q_OBJECT

    QWidget *zone = Q_NULLPTR;

    qreal valueZ;
    qreal valueZoom;

public:
    RTCRenderWidget(QWidget *zone);
    ~RTCRenderWidget();

    void onDrawImageFromRenderer(QImage image);
    void wheelEventZoomVideo(QVariant *dataStruct);
    void eventMoveVideo(QVariant *dataStruct);

private:
    void initUI();
    VideoOutput *videoOutput;
};

#endif  // RTCRENDERWIDGET_H
