#ifndef A_VIDEOWALL_H
#define A_VIDEOWALL_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"
#include "Site/site.h"
#include "VideoWall/c_videowall.h"

class C_VideoWall;
class A_VideoWall : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
  Site *workingSite = Q_NULLPTR;
  CamSite *siteCameras;

 public:
  A_VideoWall(Control *ctrl);
  void changeControl(Control *ctrl);
  C_VideoWall *control() { return (C_VideoWall *)ctrl; }
  CamSite *getSiteCameras() const;
  void setSiteCameras(CamSite *value);
  void loadSiteCameras();
  void siteChanged();
};

#endif  // ABSTRACTION_H
