#include "PlaybackImageQueue.h"

PlaybackImageQueue::PlaybackImageQueue()
{
}

void PlaybackImageQueue::enqueue(QImage newImage) {
    int size = getSize();
    int fps = 25;

    if (size > 10*fps) {
        QThread::msleep(1000);
        qDebug() << getName() << "::Image Enqueue size=" << size << " .... EMPTY queue";
        empty();
    }
    queueUpdate.lock(); 
    hashmap.insert(imageIndex++, newImage);
    queueUpdate.unlock();
}

QString PlaybackImageQueue::getName() const
{
    return name;
}

void PlaybackImageQueue::setName(const QString &value)
{
    name = value;
}

QImage PlaybackImageQueue::dequeue(long timeout) {
    QImage image;
    queueUpdate.lock();
    if(hashmap.size() >0 ) {
        quint64 imageIndex = hashmap.firstKey();
        image = hashmap.take(imageIndex);
    }
    queueUpdate.unlock();
    return image;
}

void PlaybackImageQueue::empty() {
    queueUpdate.lock();
    hashmap.clear();
    queueUpdate.unlock();
}
