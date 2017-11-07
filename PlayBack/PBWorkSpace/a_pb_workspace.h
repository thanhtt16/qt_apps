#ifndef A_PB_WORKSPACE_H
#define A_PB_WORKSPACE_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"
#include "Site/site.h"
#include "c_pb_workspace.h"

class C_PBWorkSpace;
class A_PBWorkSpace : public Abstraction {
  Q_OBJECT
  // A ref on the control facet

 public:
  A_PBWorkSpace(Control *ctrl);
  void changeControl(Control *ctrl);
  C_PBWorkSpace *control() { return (C_PBWorkSpace *)ctrl; }
  void siteChanged();
};

#endif  // ABSTRACTION_H
