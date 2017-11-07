#ifndef CONTROL_H
#define CONTROL_H
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVariant>
#include <typeinfo>
#include "PacModel/abstraction.h"
#include "PacModel/presentation.h"
#include "message.h"
class Presentation;
class Abstraction;
class Control : public QObject {
  Q_OBJECT
 private:
  // For each agent which is created has unique ID
  long idCounter = 0;

 protected:
  long id;
  Abstraction* abst;
  Presentation* pres;
  // To easily exchange messages it is usefull (sometimes) to
  QString Name = "";
  Control* parent;
  AppMessage Message;

 public:
  //
  Control(Control* ctrl);

  long getID() { return id; }
  Abstraction* getA() { return abst; }
  Presentation* getP() { return pres; }

  // To easily exchange messages it is usefull (sometimes) to
  QString getName() { return Name; }
  QString getClass() { return typeid(*this).name(); }

  /**
       * Method which change presentation and abstraction facet of this control.
       * Attention : this method also change the control attached with the
    *presentation and the abstraction.
       * @param pres a ref on the new  presentation
       * @param abst a ref on the new abstraction
       **/
  void changeFacets(Presentation* pres, Abstraction* abst);
  /**
       * Method to receive a message from the Presentation Facet.
       * @param message    : A string which describe the request
       * @param attachment : A ref on an eventual object necessary to treat the
    *request
       **/
  virtual void newUserAction(int message, QVariant* attachment);
  /**
   * Method to receive a message from the Astraction Facet.
   * @param message    : A string which describe the request
   * @param attachment : A ref on an eventual object necessary to treat the
   *request
   **/
  virtual void newSystemAction(int message, QVariant* attachment);

  /**
   * Method to receive a message from an other agent.
   * @param message    : A string which describe the request
   * @param attachment : A ref on an eventual object necessary to treat the
   *request
   **/
  virtual void newAction(int message, QVariant* attachment);
};

#endif  // CONTROL_H
