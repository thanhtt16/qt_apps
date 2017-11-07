#ifndef CAM9MAPQUEUE_H
#define CAM9MAPQUEUE_H

#include <QMap>
#include <QByteArray>
#include <QThread>
#include <QDebug>
#include <QMutex>

class Cam9MapQueue
{
private:
    QMap<quint64, QByteArray> hashmap;
    quint64 dequeueIndex = 0;
    QMutex queueUpdate;
    long maxRtpSize = 0;
    QString name = "MapQueue";
    bool working = false;

public:
    Cam9MapQueue();
    QByteArray dequeue();
    void enqueue(quint64 index, QByteArray rtpData);
    void next();
    quint64 getNextPakageNumber();
    QString getName();
    void setName(const QString &value);
    void empty();
    int getSize() { int size; queueUpdate.lock(); size=hashmap.size(); queueUpdate.unlock(); return size;}
    bool isWorking();
    void setWorking(bool value);
};

#endif // CAM9MAPQUEUE_H
