#include "a_cam9rtcplayeroverlay.h"

/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_Cam9RTCPlayerOverlay::A_Cam9RTCPlayerOverlay(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_Cam9RTCPlayerOverlay::changeControl(Control* ctrl) { this->ctrl = ctrl; }
