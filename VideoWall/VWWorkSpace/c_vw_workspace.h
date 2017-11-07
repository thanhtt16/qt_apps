#ifndef C_VW_WORKSPACE_H
#define C_VW_WORKSPACE_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>
#include "../c_videowall.h"
#include "Authentication/appcontext.h"
#include "PacModel/control.h"
#include "TopControlBar/SiteSelector/c_siteselector.h"
#include "a_vw_workspace.h"
#include "message.h"
#include "p_vw_workspace.h"
#include <QSettings>
class P_VWWorkSpace;
class A_VWWorkSpace;
class C_VideoWall;
class C_SiteSelector;
class C_VWWorkSpace : public Control {
public:
    QString appName = "Video Wall";
    QWidget *zone;
    AppContext *appContext;
    C_SiteSelector *siteSelector = Q_NULLPTR;
    bool firstShow = true;
    C_VideoWall *getParent() { return (C_VideoWall *)this->parent; }
    P_VWWorkSpace *presentation() { return (P_VWWorkSpace *)pres; }
    A_VWWorkSpace *abstraction() { return (A_VWWorkSpace *)abst; }
    C_VWWorkSpace(Control *ctrl, QWidget *zone);
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
    QString getAppName() const;
    void setAppName(const QString &value);
};

#endif  // CONTROL_H
