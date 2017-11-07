#include "a_pb_workspace.h"
#include "message.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/

void A_PBWorkSpace::siteChanged() {}

A_PBWorkSpace::A_PBWorkSpace(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_PBWorkSpace::changeControl(Control* ctrl) { this->ctrl = ctrl; }
