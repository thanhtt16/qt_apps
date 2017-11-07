#include "cam9mapqueue.h"

bool Cam9MapQueue::isWorking()
{
    return working;
}

void Cam9MapQueue::setWorking(bool value)
{
    queueUpdate.lock();
    working = value;
    queueUpdate.unlock();
}

Cam9MapQueue::Cam9MapQueue()
{
    this->name = name;
    this->dequeueIndex = 0;
}

quint64 Cam9MapQueue::getNextPakageNumber()
{
    return dequeueIndex;
}

QString Cam9MapQueue::getName()
{
    return name;
}

void Cam9MapQueue::setName(const QString &value)
{
    name = value;
}

void Cam9MapQueue::enqueue(quint64 index, QByteArray rtpData) {
    int size = getSize();
    int fps = 25;
    queueUpdate.lock();
    if (isWorking() && (index  >= dequeueIndex)) {
        hashmap.insert(index, rtpData);
    } else {
        qDebug() << getName() << "DROP!! incomming index=" << index << " dequeue index=" << dequeueIndex;
    }
    queueUpdate.unlock();

    if (size > 5 * fps) {
        qDebug() << getName() << " size=" << size << " .... EMPTY queue";
    }
}

QByteArray Cam9MapQueue::dequeue() {
    QByteArray rtpData;
    int size = getSize();
    queueUpdate.lock();
//    qDebug() << name <<"Cam9MapQueue::dequeue::[" << dequeueIndex << "] from " << hashmap.keys();

    rtpData = hashmap.take(dequeueIndex);
    if (!rtpData.isNull() && !rtpData.isEmpty()) {
        dequeueIndex++;
    }
    queueUpdate.unlock();
    return rtpData;
}


void Cam9MapQueue::next() {
    queueUpdate.lock();
    hashmap.remove(dequeueIndex);
    if (hashmap.size() > 0) {
        quint64 firstKey = hashmap.firstKey();
        qDebug() << "Cam9MapQueue::NEXT dequeueIndex + " << firstKey - dequeueIndex << " = " << firstKey;
        dequeueIndex = firstKey;
    }
    else {
        dequeueIndex++;
        //        qDebug() << "Cam9MapQueue::NEXT dequeueIndex + 1 = " << dequeueIndex;
    }
    queueUpdate.unlock();
}

void Cam9MapQueue::empty() {
    queueUpdate.lock();
    hashmap.clear();
    dequeueIndex = 0;
    queueUpdate.unlock();
}
