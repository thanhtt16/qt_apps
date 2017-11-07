#ifndef PLAYBACKRENDERWIDGET_H
#define PLAYBACKRENDERWIDGET_H

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
#include "M3U8Linker.h"

using namespace QtAV;

class PlaybackRenderWidget : public QWidget {
    Q_OBJECT

    qreal valueZ;
    qreal valueZoom;

public:
    PlaybackRenderWidget(QWidget *parent = 0);
    ~PlaybackRenderWidget();

    void onDrawImageFromRenderer(QImage image);
    void wheelEventZoomVideo(QVariant *dataStruct);
    void eventMoveVideo(QVariant *dataStruct);

private:
    void initUI();
    VideoOutput *videoOutput;
};

#endif  // PLAYBACKRENDERWIDGET_H
