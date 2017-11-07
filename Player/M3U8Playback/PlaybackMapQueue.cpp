#include "PlaybackMapQueue.h"

PlaybackMapQueue::PlaybackMapQueue()
{
    this->name = name;
    this->dequeueIndex = 0;
}

quint64 PlaybackMapQueue::getNextPakageNumber()
{
    return dequeueIndex;
}

QString PlaybackMapQueue::getName() const
{
    return name;
}

void PlaybackMapQueue::setName(const QString &value)
{
    name = value;
}

void PlaybackMapQueue::updateLastLinkQueue(quint64 linkIndex) {
    queueUpdate.lock();
    lastLinkIndex = linkIndex;

//    quint64 distance = lastLinkIndex - dequeueIndex;
//    if (distance < 0) {
//        dequeueIndex = lastLinkIndex-5;
//    } else {
//        while ((hashmap.size() > 0) && (lastLinkIndex - dequeueIndex > 5)) { // keep at most 5 waiting link
//            hashmap.take(dequeueIndex);
//            dequeueIndex++;
//        }
//    }
    queueUpdate.unlock();
}

void PlaybackMapQueue::enqueue(quint64 index, QByteArray rtpData) {
    int size = getSize();
    int fps = 25;

    queueUpdate.lock();
    if (index  >= dequeueIndex)
    {
        hashmap.insert(index, rtpData);
        qDebug() << "PlaybackMapQueue::ENQUEUE " << rtpData.size();
    }
    else {
        qDebug() << getName() << "DROP!! incomming index=" << index << " dequeue index=" << dequeueIndex;
    }
    queueUpdate.unlock();

    if (size > 5 * fps) {
        QThread::msleep(1000);
        qDebug() << getName() << " size=" << size << " .... EMPTY queue";
        empty();
    }
}

QByteArray PlaybackMapQueue::dequeue() {
    QByteArray rtpData;
    int size = getSize();
    queueUpdate.lock();
    rtpData = hashmap.take(dequeueIndex);    
    if (!rtpData.isNull() && !rtpData.isEmpty()) {
        if (size > 100)
            qDebug() << "PlaybackMapQueue::DEQUEUE" << getName() << " count=" << size;
        dequeueIndex++;
    }
    queueUpdate.unlock();
    return rtpData;
}


void PlaybackMapQueue::next() {
    queueUpdate.lock();
    hashmap.remove(dequeueIndex);
    if (hashmap.size() > 0)
        dequeueIndex = hashmap.firstKey();
    else dequeueIndex++;
    queueUpdate.unlock();
}

void PlaybackMapQueue::empty() {
    queueUpdate.lock();
    int size = hashmap.size();
    if (size > 0)
        hashmap.clear();
    dequeueIndex = lastLinkIndex;
    queueUpdate.unlock();
}
