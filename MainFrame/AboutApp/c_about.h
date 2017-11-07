#ifndef C_About_H
#define C_About_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>

#include "c_about.h"
#include "a_about.h"
#include "p_about.h"
#include "PacModel/control.h"
#include "message.h"
#include "Authentication/appcontext.h"
#include "MainFrame/c_mainframe.h"
class P_About;
class A_About;
class C_MainFrame;
class C_About : public Control {
    QWidget* zone;

public:
    AppContext *appContext = Q_NULLPTR;
    C_About(Control* ctrl, QWidget* zone);
    P_About * presentation() {return (P_About *) this->pres;}
    A_About * abstraction() {return (A_About *) this->abst;}
    C_MainFrame *getParent() { return (C_MainFrame *)this->parent; }


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
};

#endif  // CONTROL_H
