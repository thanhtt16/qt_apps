#include "a_pb_datamaptab.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_PBDataMapTab::A_PBDataMapTab(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_PBDataMapTab::changeControl(Control* ctrl) { this->ctrl = ctrl; }

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
