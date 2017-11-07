#include "PacAgentTemplate/p_classname.h"

/**
     * Generic method to override for updating the presention.
     **/

P_ClassName::P_ClassName(Control *ctrl, QWidget *zone) : Presentation(ctrl) {
  // init gui object
  Q_UNUSED(zone)
}

void P_ClassName::update() {}

QObject *P_ClassName::getZone(int zoneId) {
  switch (zoneId) {
    case 1:
      return Q_NULLPTR;
    default:
      return Q_NULLPTR;
  }
}

/**
     * Method to create a new presentation exactly equals (field by field) to
  *this presentation.
     * @return the created clone
     **/
//    public Presentation getClone(){
//        try{
//            return (Presentation)clone();
//        } catch(Exception e) {System.out.println("ERROR: can't duplicate a
//        presentation.");}
//        return null;
//    }
