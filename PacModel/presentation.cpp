#include "presentation.h"


/*
     * Contructor. Register the reference on the control.
     * @param ctrl The reference on the control attached with this facet
     */

/**
     * Change the control of this presentation
     * @param ctrl the new control for this Presentation facet
     **/

Presentation::Presentation(Control *ctrl)
{
  this->ctrl = ctrl;
}

void Presentation::changeControl(Control *ctrl){
    this->ctrl = ctrl;
}

/**
     * Generic method to override for updating the presention.
     **/
void Presentation::update(){

}

/**
     * Method to create a new presentation exactly equals (field by field) to this presentation.
     * @return the created clone
     **/
//    public Presentation getClone(){
//        try{
//            return (Presentation)clone();
//        } catch(Exception e) {System.out.println("ERROR: can't duplicate a presentation.");}
//        return null;
//    }
