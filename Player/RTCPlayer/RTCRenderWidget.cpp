#include "RTCRenderWidget.h"

RTCRenderWidget::RTCRenderWidget(QWidget *zone) {
  this->zone = zone;

  QtAV::setLogLevel(QtAV::LogOff);
  QtAV::setFFmpegLogLevel(0);
  this->videoOutput = new VideoOutput(this->zone);
  this->videoOutput->setOutAspectRatioMode(
      VideoOutput::RendererAspectRatio);
  QHBoxLayout *layout = new QHBoxLayout();
  layout->setMargin(0);
  layout->setSpacing(0);
  this->zone->setLayout(layout);
  layout->addWidget(videoOutput->widget());
  videoOutput->widget()->setMouseTracking(true);
  videoOutput->widget()->installEventFilter(this->zone);
}

void RTCRenderWidget::onDrawImageFromRenderer(QImage image) {
  videoOutput->receive(VideoFrame(image));
}

RTCRenderWidget::~RTCRenderWidget() {}

void RTCRenderWidget::wheelEventZoomVideo(QVariant *dataStruct) {
  DataZoomVideo data;
  data = dataStruct->value<DataZoomVideo>();

  QPoint dp = data.pixelDelta;
  qreal deg = data.deg;
  QPoint pos = data.pos;

  QPointF p = videoOutput->widget()->mapFrom(this->zone, pos);
  QPointF fp = videoOutput->mapToFrame(p);

  if (fp.x() < 0) fp.setX(0);
  if (fp.y() < 0) fp.setY(0);
  if (fp.x() > videoOutput->videoFrameSize().width())
    fp.setX(videoOutput->videoFrameSize().width());
  if (fp.y() > videoOutput->videoFrameSize().height())
    fp.setY(videoOutput->videoFrameSize().height());

  QRectF viewport = QRectF(
      videoOutput->mapToFrame(QPointF(0, 0)),
      videoOutput->mapToFrame(QPointF(videoOutput->rendererWidth(),
                                        videoOutput->rendererHeight())));
  qreal zoom = 1.0 + deg * 3.14 / 180.0;
  if (!dp.isNull()) {
    zoom = 1.0 + (qreal)dp.y() / 100.0;
    valueZoom = zoom;
  }
  static qreal z = 1.0;
  z *= zoom;
  if (z < 1.0) z = 1.0;
  if (z > 28.0) z = 1.0;
  valueZ = z;
  qreal x0 = fp.x() - fp.x() / z;
  qreal y0 = fp.y() - fp.y() / z;
  videoOutput->setRegionOfInterest(
      QRectF(x0, y0, qreal(videoOutput->videoFrameSize().width()) / z,
             qreal(videoOutput->videoFrameSize().height()) / z));
  return;
  QTransform m;
  m.translate(fp.x(), fp.y());
  m.scale(1.0 / zoom, 1.0 / zoom);
  m.translate(-fp.x(), -fp.y());
  QRectF r = m.mapRect(videoOutput->realROI());
  videoOutput->setRegionOfInterest(
      (r | m.mapRect(viewport)) &
      QRectF(QPointF(0, 0), videoOutput->videoFrameSize()));
}

void RTCRenderWidget::eventMoveVideo(QVariant *dataStruct) {
  DataMoveVideo dataMove;
  dataMove = dataStruct->value<DataMoveVideo>();
  QPoint pos = dataMove.pos;
  if (pos.x() > 0 && pos.y() >= 0 &&
      pos.x() < videoOutput->widget()->width() &&
      pos.y() < videoOutput->widget()->height()) {
    QPointF p = videoOutput->widget()->mapFrom(this->zone, pos);
    QPointF fp = videoOutput->mapToFrame(p);
    if (fp.x() < 0) fp.setX(0);
    if (fp.y() < 0) fp.setY(0);
    if (fp.x() > videoOutput->videoFrameSize().width())
      fp.setX(videoOutput->videoFrameSize().width());
    if (fp.y() > videoOutput->videoFrameSize().height())
      fp.setY(videoOutput->videoFrameSize().height());
    QRectF viewport = QRectF(
        videoOutput->mapToFrame(QPointF(0, 0)),
        videoOutput->mapToFrame(QPointF(videoOutput->rendererWidth(),
                                          videoOutput->rendererHeight())));

    qreal x0 = fp.x() - fp.x() / valueZ;
    qreal y0 = fp.y() - fp.y() / valueZ;
    videoOutput->setRegionOfInterest(
        QRectF(x0, y0, qreal(videoOutput->videoFrameSize().width()) / valueZ,
               qreal(videoOutput->videoFrameSize().height()) / valueZ));
    return;
    QTransform m;
    m.translate(fp.x(), fp.y());
    m.scale(1.0 / valueZoom, 1.0 / valueZoom);
    QRectF r = m.mapRect(videoOutput->realROI());
    videoOutput->setRegionOfInterest(
        (r | m.mapRect(viewport)) &
        QRectF(QPointF(0, 0), videoOutput->videoFrameSize()));
  }
}
