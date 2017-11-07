#ifndef M3U8TOQUEUE_H
#define M3U8TOQUEUE_H

#include <QObject>
#include <QString>
#include <QQueue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

class M3U8ToQueue : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager manager;
    QQueue<QString> tsUrls;
    QString m3u8Url;
    QString baseUrl;
    bool isFetching = false;

public:
    explicit M3U8ToQueue(QObject *parent = nullptr);
    void setNewSource(QString tsUrl);
    void enqueueTsUrl(QString tsUrl);
    QString dequeueTsUrl();
    void parseM3u8(QString data);
    void startFetch(QString url);

Q_SIGNALS:

public Q_SLOTS:
    void onFetchFinished(QNetworkReply *reply);
};

#endif // M3U8TOQUEUE_H
