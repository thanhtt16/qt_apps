#ifndef C_CAM9PLAYER_H
#define C_CAM9PLAYER_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <QWidget>
#include "Authentication/appcontext.h"
#include "Cam9PlayerOverlay/c_cam9playeroverlay.h"
#include "PacModel/control.h"
#include "PlayBack/PBWorkSpace/c_pb_workspace.h"
#include "PlayBack/Recorder/c_pb_record.h"
#include "VideoWall/VWWorkSpace/c_vw_workspace.h"
#include "a_cam9player.h"
#include "message.h"
#include "p_cam9player.h"
class A_Cam9Player;
class P_Cam9Player;
class C_VWWorkSpace;
class C_PBWorkSpace;
class C_PBRecord;
class C_Cam9PlayerOverlay;

class C_Cam9Player : public Control {
    Q_OBJECT

public:
    QString appName;
    QString displayName;
    QString modePlayerCurrent = "SD";
    QString getAppName() { return appName; }
    QString getDisplayName() { return displayName; }
    void setAppName(QString name) { this->appName = name; }
    void setDisplayName(QString name) {
        this->displayName = name;
        Q_EMIT setPlayerDisplayName(name);
    }
    void resetPlayerSpeed();

    QWidget* zone;
    AppContext* appContext;
    C_PBWorkSpace* getPbWorkSpace() { return (C_PBWorkSpace*)this->parent; }

    A_Cam9Player* abstraction() { return (A_Cam9Player*)this->abst; }
    P_Cam9Player* presentation() { return (P_Cam9Player*)this->pres; }
    C_Cam9Player(Control* ctrl, QWidget* zone);

    C_Cam9PlayerOverlay* cPlayerOverlay = Q_NULLPTR;
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

    QString getModePlayerCurrent() const;

Q_SIGNALS:
    void setPlayerDisplayName(QString displayName);
};

#endif  // C_CAM9PLAYER_H
