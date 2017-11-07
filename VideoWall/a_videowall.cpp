#include "VideoWall/a_videowall.h"
#include "message.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/

CamSite* A_VideoWall::getSiteCameras() const { return siteCameras; }

void A_VideoWall::loadSiteCameras() {
  // load camera list
  //  QVariant* camSiteData = new QVariant();
  //  control()->newSystemAction(Message.APP_CAM_SITE_GET, camSiteData);
  //  this->siteCameras = camSiteData->value<CamSite*>();
}

void A_VideoWall::setSiteCameras(CamSite* value) { siteCameras = value; }

void A_VideoWall::siteChanged() { this->loadSiteCameras(); }

A_VideoWall::A_VideoWall(Control* ctrl) : Abstraction(ctrl) {
  //  loadSiteCameras();
}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_VideoWall::changeControl(Control* ctrl) { this->ctrl = ctrl; }

/**
 * Method to create a new abstraction exactly equals to this abstraction.
 * @return the created clone
 **/
// public Abstraction getClone(){
//    try{
//        return (Abstraction)clone();
//    } catch(Exception e) {System.out.println("ERROR: can't duplicate an
//    abstraction.");}
//    return null;
//}
