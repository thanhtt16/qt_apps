#ifndef A_ABOUT_H
#define A_ABOUT_H

#include <QObject>
#include "c_about.h"
#include "PacModel/control.h"

class C_About;
class A_About : public Abstraction {
    Q_OBJECT
public:
    C_About *control() { return (C_About *)ctrl; }
    A_About(Control *ctrl);

};

#endif  // ABSTRACTION_H
