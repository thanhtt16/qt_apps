#ifndef PLAYBACKIMAGEQUEUE_H
#define PLAYBACKIMAGEQUEUE_H

#include <QMap>
#include <QImage>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QException>
#include <QDateTime>

class PlaybackImageQueue
{
private:
    QMap<quint64, QImage> hashmap;
    QMutex queueUpdate;
    QString name = "Image Queue";
    quint64 imageIndex = 0;

public:
    PlaybackImageQueue();
    QImage dequeue(long timeout);
    void enqueue(QImage newImage);
    QString getName() const;
    void setName(const QString &value);
    void empty();
    int getSize() { int size; queueUpdate.lock(); size=hashmap.size(); queueUpdate.unlock(); return size;}
};

#endif // PLAYBACKIMAGEQUEUE_H
