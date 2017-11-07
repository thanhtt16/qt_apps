#include "a_cam9player.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_Cam9Player::A_Cam9Player(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_Cam9Player::changeControl(Control* ctrl) { this->ctrl = ctrl; }
