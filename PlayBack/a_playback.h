#ifndef A_PlayBack_H
#define A_PlayBack_H

#include <QObject>
#include "Camera/camsite.h"
#include "PacModel/control.h"
#include "PlayBack/c_playback.h"
#include "Site/site.h"

class C_PlayBack;
class A_PlayBack : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
  Site *workingSite = Q_NULLPTR;
  CamSite *siteCameras;

 public:
  A_PlayBack(Control *ctrl);
  void changeControl(Control *ctrl);
  C_PlayBack *control() { return (C_PlayBack *)ctrl; }
  CamSite *getSiteCameras() const;
  void setSiteCameras(CamSite *value);
  void loadSiteCameras();
  void siteChanged();
};

#endif  // ABSTRACTION_H
