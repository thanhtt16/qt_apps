#include "seekbarwidget.h"

SeekBarWidget::SeekBarWidget(QWidget *parent) : QWidget(parent) {

    setStyleSheet("background-color:red;");
    pointerTimer = new QTimer(this);
    connect(pointerTimer, &QTimer::timeout, this, &SeekBarWidget::onMovePointer);
}

void SeekBarWidget::stopMovePointer() {
    if (this->pointerTimer->isActive()) {
        this->pointerTimer->stop();
    }
}


//Cập nhật timestamp khi seek đên vi tri moi
void SeekBarWidget::updateTimeStamp(long timestamp, int duration){
    startTimestamp = timestamp;
    currentTimestamp = startTimestamp;
    currentValue = 0;
    endTimestamp = startTimestamp + duration;

    startTimeString =
            QDateTime::fromSecsSinceEpoch(startTimestamp).toString("hh:mm:ss");
    endTimeString =
            QDateTime::fromSecsSinceEpoch(endTimestamp).toString("hh:mm:ss");

    this->totalDuration = duration;
    this->update();
}

//seek bar bắt đầu tăng giá trị lên 1s +1
void SeekBarWidget::onMovePointer() {
    if (currentTimestamp < endTimestamp) {
        currentTimestamp += 1;
    }
    TimeRange timeRangeCurrent;
    timeRangeCurrent.starttime = currentTimestamp;
    timeRangeCurrent.length = endTimestamp - currentTimestamp;

    Q_EMIT updateTimeRangeCurrent(timeRangeCurrent);
    currentPercent = ((float)(currentTimestamp - startTimestamp) / (float)(this->totalDuration)); // so phan tram hien tai
    pointerX = leftPadding + realWidth * currentPercent;

    //    qDebug() << "POINT X " << pointerX << currentPercent;
    this->update();
}

//gọi ở ngoài để truyền vào
void SeekBarWidget::drawDataMapFrom(QList<int> listMapOfHour) {
    this->listStateOfHour = listMapOfHour;
    this->totalDuration = 3600;
    this->timeScale = 1.0;
    startMovePointer(); //bắt đầu vẽ thời gian bắt đầu thời gian kết thúc
    update();
}

void SeekBarWidget::startMovePointer() {
    if (!this->pointerTimer->isActive()) {
        pointerTimer->start(1000);
    }
}


void SeekBarWidget::mouseMoveEvent(QMouseEvent *event) {
    handleMousePositon(event->pos());
}

void SeekBarWidget::mousePressEvent(QMouseEvent *event) {
    handleMousePositon(event->pos());
}

void SeekBarWidget::mouseReleaseEvent(QMouseEvent *event) {
    handleMousePositon(event->pos());
}



void SeekBarWidget::handleMousePositon(QPointF pos) {
    float posX = pos.x();
    if (posX < leftPadding) {
        posX = leftPadding;
    } else if (posX > (leftPadding + realWidth)) {
        posX = leftPadding + realWidth;
    }
    //de lay gia tri can tru di leftpadding

    currentPercent = (posX - leftPadding) / realWidth;
    currentPercent = round(currentPercent * 1000) / 1000;
    //    currentTimestamp = startTimestamp  + currentPercent * 3600;


    currentValue = (posX - leftPadding) * (totalDuration/realWidth) ; //lay ra gia tri thuc hien tai 0 - 3600
    currentTimestamp = startTimestamp  + currentValue;
    qDebug() << Q_FUNC_INFO << "CURRENT TIMESTAMP SEEKBAR----"<<currentTimestamp << "POSX " << posX << currentValue << "START" << startTimestamp+ currentValue;
    pointerX = leftPadding + realWidth * currentPercent;

    Q_EMIT seekNewPositon(currentTimestamp, endTimestamp - currentTimestamp);
    this->update();
}


void SeekBarWidget::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    realHeight = (float)this->height() - bottomPadding;
    realWidth = (float)this->width() - (leftPadding + rightPadding);
    drawBackground(painter);
    drawMap(painter); // vẽ datamap của playback
    drawBar(painter);
    drawPointer(painter);
    drawText(painter);
    painter.end();
}

//vẽ và setbackground khung seek bar ban đầu
void SeekBarWidget::drawBackground(QPainter &painter) {
    painter.fillRect(QRect(0, 0, this->width(), this->height()),
                     QColor("#eff0f5"));
    painter.fillRect(QRect(leftPadding, 0, realWidth, realHeight),
                     QColor("#8a8a92"));
}


//Hàm để vẽ data map của dữ liệu playback
void SeekBarWidget::drawMap(QPainter &painter) {
    mapX = leftPadding;
    int size = listStateOfHour.size();
    if (size > 0) {
        float unit = (float)(realWidth * this->timeScale) / 3600;
        for (int index = 0; index < size; ++index) {
            QRectF rect(mapX, 0, unit, realHeight);
            mapX += unit;
            int value = listStateOfHour.at(index);
            QColor color;
            if(value != -1){
                color = value == 1 ? QColor("#4cd964") : QColor("#ff3b30");
            }else{
                color = QColor("#eff0f5");
            }
            painter.fillRect(rect, color);
        }
    }
}

void SeekBarWidget::drawBar(QPainter &painter) {
    painter.fillRect(QRect(leftPadding, realHeight - 5, realWidth, 5),
                     QColor("#fdf6aa"));
    painter.fillRect(
                QRect(leftPadding, realHeight - 5, pointerX - leftPadding, 5),
                QColor("#1e63ee"));
}

void SeekBarWidget::drawPointer(QPainter &painter) {
    QPainterPath path;
//    path.moveTo(pointerX, realHeight - 35);
//    path.lineTo(pointerX, realHeight - 35);

    path.moveTo(pointerX - 30, realHeight - 35);
    path.lineTo(pointerX + 30, realHeight - 35);
    path.lineTo(pointerX + 30, realHeight - 10);
    path.lineTo(pointerX + 5, realHeight - 10);
    path.lineTo(pointerX, realHeight - 5);
    path.lineTo(pointerX - 5, realHeight - 10);
    path.lineTo(pointerX - 30, realHeight - 10);
    path.lineTo(pointerX - 30, realHeight - 35);
    painter.fillPath(path, QColor("#1e63ee"));
}

void SeekBarWidget::drawText(QPainter &painter) {
    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawText(QRect(0, realHeight, textWidth, textHeight),
                     startTimeString);
    painter.drawText(QRect(realWidth, realHeight, textWidth, textHeight),
                     endTimeString);
    currentTimeString =
            QDateTime::fromSecsSinceEpoch(currentTimestamp)
            .toString("hh:mm:ss");
    painter.drawText(QRect(pointerX - 30, realHeight - 30, textWidth, textHeight),
                     currentTimeString);
}


int SeekBarWidget::getStartTimestamp() const
{
    return startTimestamp;
}

void SeekBarWidget::setStartTimestamp(int value)
{
    startTimestamp = value;
}

int SeekBarWidget::getTimeDuration() const
{
    return timeDuration;
}

void SeekBarWidget::setTimeDuration(int value)
{
    timeDuration = value;
}


QList<int> SeekBarWidget::getListStateOfHour() const
{
    return listStateOfHour;
}

void SeekBarWidget::setListStateOfHour(const QList<int> &value)
{
    listStateOfHour = value;
}
