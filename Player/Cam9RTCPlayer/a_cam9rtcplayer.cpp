#include "a_cam9rtcplayer.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_Cam9RTCPlayer::A_Cam9RTCPlayer(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_Cam9RTCPlayer::changeControl(Control* ctrl) { this->ctrl = ctrl; }
