#ifndef CAM9M3U8QUEUE_H
#define CAM9M3U8QUEUE_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class Cam9M3u8Queue : public QObject
{
    Q_OBJECT
private:
    QMutex queueMutex;
    QNetworkAccessManager manager;
    QQueue<QString> tsUrls;
    QString m3u8Url;
    QString baseUrl;
    bool isFetching = false;

public:
    explicit Cam9M3u8Queue();
    void setNewSource(QString tsUrl);
    void enqueue(QString tsUrl);
    QString dequeue();
    void parseM3u8(QString data);
    void startFetch(QString url);

Q_SIGNALS:

public Q_SLOTS:
    void onFetchFinished(QNetworkReply *reply);
};

#endif // CAM9M3U8QUEUE_H
