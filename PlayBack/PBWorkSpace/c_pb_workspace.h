#ifndef C_PB_WORKSPACE_H
#define C_PB_WORKSPACE_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "../c_playback.h"
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "PlayBack/Recorder/c_pb_record.h"
#include "SeekBar/c_pb_seekbar.h"
#include "TopControlBar/SiteSelector/c_siteselector.h"
#include "a_pb_workspace.h"
#include "message.h"
#include "p_pb_workspace.h"
class P_PBWorkSpace;
class A_PBWorkSpace;
class C_PlayBack;
class C_SiteSelector;
class C_PBSeekBar;
class C_PBWorkSpace : public Control {
public:
    QString appName = "Playback";
    QWidget *zone;
    AppContext *appContext;
    C_PBSeekBar *cSeekBar = Q_NULLPTR;
    C_PBRecord *cRecoder = Q_NULLPTR;
    C_SiteSelector *siteSelector = Q_NULLPTR;
    C_PlayBack *getParent() { return (C_PlayBack *)this->parent; }
    P_PBWorkSpace *presentation() { return (P_PBWorkSpace *)pres; }
    A_PBWorkSpace *abstraction() { return (A_PBWorkSpace *)abst; }
    C_PBWorkSpace(Control *ctrl, QWidget *zone);
    /**
        * Method to receive a message from the Presentation Facet.
        * @param message    : A string which describe the request
        * @param attachment : A ref on an eventual object necessary to treat the
    *request
        **/
    void newUserAction(int message, QVariant *attachment) override;
    /**
    * Method to receive a message from the Astraction Facet.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
    void newSystemAction(int message, QVariant *attachment) override;

    /**
    * Method to receive a message from an other agent.
    * @param message    : A string which describe the request
    * @param attachment : A ref on an eventual object necessary to treat the
    *request
    **/
    void newAction(int message, QVariant *attachment) override;
};

#endif  // CONTROL_H
