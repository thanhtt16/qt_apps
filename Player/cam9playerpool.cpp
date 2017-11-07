#include "cam9playerpool.h"

Cam9PlayerPool::Cam9PlayerPool() : QObject(Q_NULLPTR) {}
Cam9PlayerPool::~Cam9PlayerPool() {}

C_Cam9Player *Cam9PlayerPool::getPlayer(Control *control, QWidget *zone,
                                        QString appName, QString displayName) {
    bool playerFound = false;

    C_Cam9Player *foundPlayer = Q_NULLPTR;
    for (int index = 0; index < playerPoolPlayback.size(); ++index) {
        C_Cam9Player *player = playerPoolPlayback.at(index)->value<C_Cam9Player*>();
        if (player->getAppName() == appName &&
                player->getDisplayName() == displayName) {
            foundPlayer = player;
            playerFound = true;
            break;
        }
    }
    if (!playerFound) {
        //    for (int index = 0; index < playerPool.size(); ++index) {
        //      Cam9Player *player = playerPool.at(index);
        //      if (player->getAppName() != appName) {
        //        QString oldOwner = player->getAppName();
        //        foundPlayer = player;
        //        foundPlayer->setAppName(appName);
        //        playerFound = true;
        //        qDebug() << Q_FUNC_INFO << "Cuop cua" << oldOwner << "cho" <<
        //        appName
        //                 << displayName;
        //        break;
        //      }
        //    }
        foundPlayer = createNewPlayer(control, zone);
        foundPlayer->setAppName(appName);
        foundPlayer->setDisplayName(displayName);
    }
    return foundPlayer;
}


C_Cam9RTCPlayer *Cam9PlayerPool::getCam9RTCPlayer(Control *control, QWidget *zone,
                                                  QString appName, QString displayName) {
    bool playerFound = false;

    C_Cam9RTCPlayer *foundPlayer = Q_NULLPTR;
    for (int index = 0; index < playerPoolVideowall.size(); ++index) {
        C_Cam9RTCPlayer *player = playerPoolVideowall.at(index)->value<C_Cam9RTCPlayer*>();

        if (player->getAppName() == appName &&
                player->getDisplayName() == displayName) {
            foundPlayer = player;
            playerFound = true;
            break;
        }
    }
    if (!playerFound) {
        //    for (int index = 0; index < playerPool.size(); ++index) {
        //      Cam9Player *player = playerPool.at(index);
        //      if (player->getAppName() != appName) {
        //        QString oldOwner = player->getAppName();
        //        foundPlayer = player;
        //        foundPlayer->setAppName(appName);
        //        playerFound = true;
        //        qDebug() << Q_FUNC_INFO << "Cuop cua" << oldOwner << "cho" <<
        //        appName
        //                 << displayName;
        //        break;
        //      }
        //    }


        foundPlayer = createNewCam9RTCPlayer(control, zone);
        foundPlayer->setAppName(appName);
        foundPlayer->setDisplayName(displayName);
    }
    return foundPlayer;
}



void Cam9PlayerPool::playAll(QString appName){
    Q_UNUSED(appName)
    //  for (int index = 0; index < playerPool.size(); ++index) {
    //    C_Cam9Player *player = playerPool.at(index);
    //    if (player->getAppName() == appName) {
    //      player->open();
    //      player->play();
    //    }
    //  }
}

C_Cam9Player *Cam9PlayerPool::createNewPlayer(Control *control, QWidget *zone) {
    C_Cam9Player *player = new C_Cam9Player(control, zone);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<C_Cam9Player*>(player);
    playerPoolPlayback.append(dataStruct);
    return player;
}

C_Cam9RTCPlayer *Cam9PlayerPool::createNewCam9RTCPlayer(Control *control, QWidget *zone) {
    C_Cam9RTCPlayer *player = new C_Cam9RTCPlayer(control, zone);
    QVariant *dataStruct = new QVariant();
    dataStruct->setValue<C_Cam9RTCPlayer*>(player);
    playerPoolVideowall.append(dataStruct);
    return player;
}


/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void Cam9PlayerPool::newAction(int message, QVariant* attachment) {
    switch (message) {
    case Message.CREATE_NEW_CAM9_RTC_PLAYER:{


        PLAYER_DATA_STRUCT dataCreatPlayer = attachment->value<PLAYER_DATA_STRUCT>();
        Control * control = dataCreatPlayer.control;
        QWidget *zone = dataCreatPlayer.zone;
        C_Cam9RTCPlayer *newPlayer  = createNewCam9RTCPlayer(control, zone);
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<C_Cam9RTCPlayer*>(newPlayer);
        playerPoolVideowall.append(dataStruct);

    } break;

    case Message.CREATE_NEW_CAM9_PLAYER:{

        PLAYER_DATA_STRUCT dataCreatPlayer = attachment->value<PLAYER_DATA_STRUCT>();
        Control * control = dataCreatPlayer.control;
        QWidget *zone = dataCreatPlayer.zone;

        C_Cam9Player *newPlayer  = createNewPlayer(control, zone);
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<C_Cam9Player*>(newPlayer);
        playerPoolPlayback.append(dataStruct);

    } break;

    case Message.GET_CAM9_RTC_PLAYER:{
        //        Control *control, QWidget *zone,QString appName, QString displayName
        C_Cam9RTCPlayer *cPlayer = Q_NULLPTR;
        PLAYER_DATA_STRUCT playerInfo= attachment->value<PLAYER_DATA_STRUCT>();
        Control * control = playerInfo.control;
        QWidget *zone = playerInfo.zone;
        QString appName = playerInfo.appName;
        QString displayName = playerInfo.displayName;
        cPlayer = getCam9RTCPlayer(control, zone, appName, displayName);
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<C_Cam9RTCPlayer*>(cPlayer);
    }break;

    case Message.GET_CAM9_PLAYER:{
        PLAYER_DATA_STRUCT playerInfo= attachment->value<PLAYER_DATA_STRUCT>();
        Control * control = playerInfo.control;
        QWidget *zone = playerInfo.zone;
        QString appName = playerInfo.appName;
        QString displayName = playerInfo.displayName;
        C_Cam9Player *cPlayer = getPlayer(control, zone, appName, displayName);
        QVariant *dataStruct = new QVariant();
        dataStruct->setValue<C_Cam9Player*>(cPlayer);
    }break;
    }
}
