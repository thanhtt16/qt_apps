#include "PlayBack/a_playback.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/

CamSite* A_PlayBack::getSiteCameras() const { return siteCameras; }

void A_PlayBack::loadSiteCameras() {
  // load camera list
}

void A_PlayBack::setSiteCameras(CamSite* value) { siteCameras = value; }

void A_PlayBack::siteChanged() { this->loadSiteCameras(); }

A_PlayBack::A_PlayBack(Control* ctrl) : Abstraction(ctrl) { loadSiteCameras(); }

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_PlayBack::changeControl(Control* ctrl) { this->ctrl = ctrl; }

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
