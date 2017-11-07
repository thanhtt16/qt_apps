#ifndef PLAYBACKMAPQUEUE_H
#define PLAYBACKMAPQUEUE_H

#include <QMap>
#include <QByteArray>
#include <QMutexLocker>
#include <QThread>
#include <QDebug>
#include <QWaitCondition>

class PlaybackMapQueue
{
private:
    QMap<quint64, QByteArray> hashmap;
    quint64 dequeueIndex = 0;
    quint64 lastLinkIndex = 0;
    QMutex queueUpdate;

    QString name = "MapQueue";
public:
    PlaybackMapQueue();
    QByteArray dequeue();
    void enqueue(quint64 index, QByteArray rtpData);
    void updateLastLinkQueue(quint64 linkIndex);
    void next();
    quint64 getNextPakageNumber();
    QString getName() const;
    void setName(const QString &value);
    void empty();
    int getSize() { int size; queueUpdate.lock(); size=hashmap.size(); queueUpdate.unlock(); return size;}
};

#endif // PLAYBACKMAPQUEUE_H
