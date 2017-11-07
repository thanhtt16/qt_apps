#ifndef A_VW_WORKSPACE_H
#define A_VW_WORKSPACE_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"
#include "Site/site.h"
#include "c_vw_workspace.h"

class C_VWWorkSpace;
class A_VWWorkSpace : public Abstraction {
  Q_OBJECT
  // A ref on the control facet

 public:
  A_VWWorkSpace(Control *ctrl);
  void changeControl(Control *ctrl);
  C_VWWorkSpace *control() { return (C_VWWorkSpace *)ctrl; }
  void siteChanged();
};

#endif  // ABSTRACTION_H
