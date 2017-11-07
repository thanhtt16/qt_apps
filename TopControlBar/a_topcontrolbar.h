#ifndef A_TOPCONTROLBAR_H
#define A_TOPCONTROLBAR_H

#include <QObject>
#include "PacModel/control.h"
#include "Site/sitechild.h"
class C_TopControlBar;
class A_TopControlBar : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
  SiteChild *childSite;

 public:
  A_TopControlBar(Control *ctrl);
  void changeControl(Control *ctrl);
  SiteChild *getChildSite() const;
  void setChildSite(SiteChild *value);
};

#endif  // A_TOPCONTROLBAR_H
