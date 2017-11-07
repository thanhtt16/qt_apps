#ifndef CAMSTATISTIC_H
#define CAMSTATISTIC_H

#include <QObject>
#include <QString>

class CamStatistic : public QObject
{
        Q_OBJECT
        
    private:
        
        
    public:
        static QString kLogFormat;
        explicit CamStatistic(QObject *parent = 0);
        
        qint64 mReconnectStreamCounter = 0;
        
        qint64 mDisconnectStreamCounter = 0;
        qint64 mDisconnectedStreamThreshold = 0;
        
        qreal mCurrentFps;
        
        QString mLastReconnectedDateTime;
        
        QString mState = "⇅";
        QString mStyle = "";
        QString statLog(QString, QString, float, float);
};

#endif // CAMSTATISTIC_H
