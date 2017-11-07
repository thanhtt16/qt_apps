#include "PacModel/control.h"
#include <QDebug>
/**
     * Contructor. Register the father in the pac hierarchy.
     * @param ctrl The reference on the father. Generally the pac agent which
     * create this agent.
     **/
Control::Control(Control* ctrl) {
  parent = ctrl;
  QString ClassName = metaObject()->className();
  ClassName = ClassName.split(".").last();
  if (ClassName.split("C_").first() != "") {
    ClassName.truncate(3);
    QString idString = QString::number(idCounter);
    Name = ClassName.append(idString);
  } else
    qDebug() << "Bad agen Name (must start with C and be in a package) ->" +
                    getClass();
  id = idCounter++;
}
/**
     * Method which change presentation and abstraction facet of this control.
     * Attention : this method also change the control attached with the
  *presentation and the abstraction.
     * @param pres a ref on the new  presentation
     * @param abst a ref on the new abstraction
     **/

void Control::changeFacets(Presentation* pres, Abstraction* abst) {
  this->pres = pres;
  if (pres != Q_NULLPTR) pres->changeControl(this);

  this->abst = abst;
  if (abst != Q_NULLPTR) abst->changeControl(this);
}

/**
     * Method to receive a message from the Presentation Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void Control::newUserAction(int message, QVariant* attachment) {
  qDebug() << "ERROR : General User action in" << getClass()
           << "non-catched :" << QString::number(message) << attachment;
}

/**
     * Method to receive a message from the Astraction Facet.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void Control::newSystemAction(int message, QVariant* attachment) {
  qDebug() << "ERROR : General System action in" << getClass()
           << "non-catched :" << QString::number(message) << attachment;
}

/**
     * Method to receive a message from an other agent.
     * @param message    : A string which describe the request
     * @param attachment : A ref on an eventual object necessary to treat the
  *request
     **/
void Control::newAction(int message, QVariant* attachment) {
  qDebug() << "ERROR : General Internal pac action in" << getClass()
           << "non-catched :" << QString::number(message) << attachment;
}

/**
     * Method to create a new pac agent exactly equals to the agent attached to
  *this control.
     * @return the control of the created clone.
     **/
// Control Control::getClone(){
//    try{
//        Presentation 	P = Q_NULLPTR;
//        Abstraction 	A = Q_NULLPTR;
//        Control 		C = (Control)clone();

//        if (pres != null)
//            P = (Presentation)(pres.getClone());
//        if (abst != null)
//            A = (Abstraction)(abst.getClone());

//        C.changeFacets(P, A);
//        return C;
//    } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//    control.");}
//    return null;
//}
