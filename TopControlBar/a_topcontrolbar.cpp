#include "TopControlBar/a_topcontrolbar.h"
/**
 * Constructor for Abstraction facet. Register the control.
 * @param Crtl A ref on the control facet which manage this facet
 **/
A_TopControlBar::A_TopControlBar(Control* ctrl) : Abstraction(ctrl) {}

/**
 * Change the control of this abstraction
 * @param ctrl the new control for this abstraction facet
 **/
void A_TopControlBar::changeControl(Control* ctrl) { this->ctrl = ctrl; }

SiteChild* A_TopControlBar::getChildSite() const { return childSite; }

void A_TopControlBar::setChildSite(SiteChild* value) {
  childSite = value;
  qDebug() << childSite->getListSite().first()->getSiteAddress();
}
