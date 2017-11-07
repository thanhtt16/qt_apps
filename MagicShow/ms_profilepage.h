#ifndef MS_PROFILEPAGE_H
#define MS_PROFILEPAGE_H

#include <QObject>
#include "Camera/camstream.h"
#include "Camera/camprofile.h"
#include "Camera/camitem.h"
#include "Camera/camsite.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class ProfilePage : public QObject
{
        Q_OBJECT
    public:
        static ProfilePage* parseFrom(QJsonObject);
        ProfilePage(QObject *parent = 0);
        
        int getLayout() const;
        void setLayout(int);
        
        int getPage() const;
        void setPage(int);
        
        int getDuration() const;
        void setDuration(int duration);
        
        QList<CamItem*> getCamItems();
        
        void setIds(QList<int>);
        QList<int> getCamIds();
        
        QJsonObject toJsonObject();
        
        CamSite* toCamSite(CamSite*);
    private:
        int mLayout = 4;
        int mPage = 1;
        int mDuration = 60;
        QList<CamItem*> mCamItems;
        QList<int> mCamIds;
        
    Q_SIGNALS:
        
    public Q_SLOTS:
};

#endif // PROFILEPAGE_H
