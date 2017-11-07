#include "a_vw_workspace.h"
#include "message.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/

void A_VWWorkSpace::siteChanged() {}

A_VWWorkSpace::A_VWWorkSpace(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_VWWorkSpace::changeControl(Control* ctrl) { this->ctrl = ctrl; }
