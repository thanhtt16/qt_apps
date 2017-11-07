#ifndef A_SITESELECTOR_H
#define A_SITESELECTOR_H

#include <QObject>
#include "PacModel/control.h"
class C_SiteSelector;
class A_SiteSelector : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_SiteSelector(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // A_SITESELECTOR_H
