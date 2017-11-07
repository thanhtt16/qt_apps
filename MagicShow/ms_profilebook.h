#ifndef MS_PROFILEBOOK_H
#define MS_PROFILEBOOK_H

#include <QObject>
#include "MagicShow/ms_profilepage.h"
#include "Authentication/user.h"
#include "Camera/camsite.h"

class ProfileBook : public QObject
{
        Q_OBJECT
    public:
        
        static QList<ProfileBook*>* loadProfileBookOf(int, int);
        static void saveProfileBookTo(QList<ProfileBook*>*, int, int);
        static ProfileBook* parseFrom(QJsonObject);
        ProfileBook(QObject *parent);
        
        QString getName() const;
        void setName(QString);
        
        int getCurrentPage() const;
        void setCurrentPage(int);
        
        int getSiteId() const;
        void setSiteId(int);
        
        int getUserId() const;
        void setUserId(int);
        
        QList<ProfilePage*> getProfilePages();
        void setProfilePages(QList<ProfilePage*>);
        QJsonObject toJsonObject();
        
        QList<CamSite*>* getCamSites(CamSite *sourceCamSite);
        
    private:
        QString mName;
        int mUserId;
        int mSiteId;
        int mCurrentPage = 1;
        QList<ProfilePage*> mProfilePages;
    Q_SIGNALS:
        
    public Q_SLOTS:
};

#endif // PROFILEBOOK_H
