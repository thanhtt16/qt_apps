#ifndef C_Socket_H
#define C_Socket_H
#include <QString>

#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QWidget>

#include "p_socket.h"
#include "a_socket.h"
#include "PacModel/control.h"
#include "message.h"
#include "Authentication/appcontext.h"
#include "MainFrame/c_mainframe.h"
class P_Socket;
class A_Socket;
class C_MainFrame;
class C_Socket : public Control {
    QWidget* zone;

public:
    AppContext *appContext = Q_NULLPTR;
    C_Socket(Control* ctrl, QWidget* zone);
    P_Socket * presentation() {return (P_Socket *) this->pres;}
    A_Socket * abstraction() {return (A_Socket *) this->abst;}
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
