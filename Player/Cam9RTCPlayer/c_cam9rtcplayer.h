#ifndef C_Cam9RTCPlayer_H
#define C_Cam9RTCPlayer_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "Cam9RTCPlayerOverlay/c_cam9rtcplayeroverlay.h"
#include "PacModel/control.h"
#include "PlayBack/PBWorkSpace/c_pb_workspace.h"
#include "PlayBack/Recorder/c_pb_record.h"
#include "VideoWall/VWWorkSpace/c_vw_workspace.h"
#include "c_cam9rtcplayer.h"
#include "message.h"
#include "p_cam9rtcplayer.h"
class A_Cam9RTCPlayer;
class P_Cam9RTCPlayer;
class C_VWWorkSpace;
class C_PBWorkSpace;
class C_PBRecord;
class C_Cam9RTCPlayerOverlay;

class C_Cam9RTCPlayer : public Control {
    Q_OBJECT

private:
    RTCPlayer *rtcPlayer = Q_NULLPTR;
    PlaybackPlayer *playbackPlayer = Q_NULLPTR;
public:
    QString appName;
    QString displayName;

    QString getAppName() { return appName; }
    QString getDisplayName() { return displayName; }
    void setAppName(QString name) { this->appName = name; }
    void setDisplayName(QString name) {
        this->displayName = name;
        setPlayerDisplayName(name);
    }
    void resetPlayerSpeed();

    QWidget* zone;
    AppContext* appContext;
    C_VWWorkSpace* getVwWorkSpace() { return (C_VWWorkSpace*)this->parent; }
    C_PBWorkSpace* getPbWorkSpace() { return (C_PBWorkSpace*)this->parent; }

    A_Cam9RTCPlayer* abstraction() { return (A_Cam9RTCPlayer*)this->abst; }
    P_Cam9RTCPlayer* presentation() { return (P_Cam9RTCPlayer*)this->pres; }
    C_Cam9RTCPlayer(Control* ctrl, QWidget* zone);

    C_Cam9RTCPlayerOverlay* cPlayerOverlay = Q_NULLPTR;
    void show(QVariant* attachment);
    /**
        * Method to receive a message from the Presentation Facet.
        * @param message    : A string which describe the request
        * @param attachment : A ref on an eventual object necessary to treat the
    *request
        **/
    void newUserAction(int message, QVariant* attachment) override;
    /**
    * Method to receive a message from the Astraction Facet.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
    void newSystemAction(int message, QVariant* attachment) override;

    /**
    * Method to receive a message from an other agent.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
    void newAction(int message, QVariant* attachment) override;

Q_SIGNALS:
    void setPlayerDisplayName(QString displayName);
};

#endif  // C_Cam9RTCPlayer_H
