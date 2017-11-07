#include "a_setting_general.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_SettingGeneral::A_SettingGeneral(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_SettingGeneral::changeControl(Control* ctrl) { this->ctrl = ctrl; }
