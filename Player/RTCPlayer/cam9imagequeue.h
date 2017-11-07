#ifndef CAM9IMAGEQUEUE_H
#define CAM9IMAGEQUEUE_H

#include <QMap>
#include <QImage>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QException>
#include <QDateTime>

class Cam9ImageQueue
{
private:
    QMap<double, QImage> hashmap;
    QMutex queueUpdate;
    QString name = "Image Queue";
    double lastImageKey = 0.0;
    double timeWait = 40;
    QImage dequeueImage;
    bool working = false;

public:
    Cam9ImageQueue();
    void dequeue();
    void enqueue(double timestamp, QImage newImage);
    QString getName() const;
    void setName(const QString &value);
    void empty();
    int getSize() { int size; queueUpdate.lock(); size=hashmap.size(); queueUpdate.unlock(); return size;}
    double getTimeWait();
    QImage getDequeueImage();
    long getVideoTime();
    bool isWorking();
    void setWorking(bool value);
};

#endif // CAM9IMAGEQUEUE_H
