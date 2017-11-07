#include "cam9imagequeue.h"

double Cam9ImageQueue::getTimeWait()
{
    double delay;
    queueUpdate.lock();
    delay = timeWait;
    queueUpdate.unlock();
    return delay;
}

QImage Cam9ImageQueue::getDequeueImage()
{
    QImage image;
    queueUpdate.lock();
    //    dequeueImage = this->hashmap.take(this->hashmap.firstKey());
    image = dequeueImage;
    queueUpdate.unlock();
    //    qDebug()<< QDateTime::currentMSecsSinceEpoch() << this << Q_FUNC_INFO << image.size();
    return image;
}

bool Cam9ImageQueue::isWorking()
{
    return working;
}

void Cam9ImageQueue::setWorking(bool value)
{
    queueUpdate.lock();
    working = value;
    queueUpdate.unlock();
}

Cam9ImageQueue::Cam9ImageQueue()
{
}

void Cam9ImageQueue::enqueue(double timestamp, QImage newImage) {
    int size = getSize();
    int fps = 25;

    if (size > 6*fps) {
        qDebug() << getName() << "::Image Enqueue size=" << size << " .... EMPTY queue";
        empty();
    }

    queueUpdate.lock();
    if(isWorking()){
        hashmap.insert(timestamp, newImage);
    }
    queueUpdate.unlock();
}

QString Cam9ImageQueue::getName() const
{
    return name;
}

void Cam9ImageQueue::setName(const QString &value)
{
    name = value;
}

void Cam9ImageQueue::dequeue() {
    QImage image;
    double currentImageKey;
    queueUpdate.lock();
    if(hashmap.size() >0 ) {
        currentImageKey   = hashmap.firstKey();
        //        if(hashmap.size() > 25)
        //            qDebug() << name <<"Cam9MapQueue::dequeue::[" << currentImageKey <<"] from " << "SIZE" << hashmap.size();
        image = hashmap.take(currentImageKey);
    }

    if (!this->dequeueImage.isNull()) {
        long delay = currentImageKey - this->lastImageKey;
        if (delay < 0 || delay > 100) {
            this->timeWait = 40;
        } else {
            this->timeWait = delay;
        }
        this->lastImageKey = currentImageKey;
    }

    queueUpdate.unlock();

    this->dequeueImage = image;
}

void Cam9ImageQueue::empty() {
    queueUpdate.lock();
    hashmap.clear();
    queueUpdate.unlock();
}

// return video time length in second in the queue
long Cam9ImageQueue::getVideoTime() {
    long videoTime = 1;
    queueUpdate.lock();
    if (hashmap.size() > 0) {
        double firstTimestamp = hashmap.firstKey();
        double lastTimestamp = hashmap.lastKey();
        videoTime = lastTimestamp - firstTimestamp + 1;
    }
    queueUpdate.unlock();
    if (videoTime < 0) videoTime = 10000000;
    return videoTime;
}
