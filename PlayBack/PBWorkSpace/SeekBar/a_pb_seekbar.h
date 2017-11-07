#ifndef A_PBSeekBar_H
#define A_PBSeekBar_H

#include <QObject>
#include "PacModel/control.h"
class C_PBSeekBar;
class A_PBSeekBar : public Abstraction {
  Q_OBJECT
  // A ref on the control facet
 private:
 public:
  A_PBSeekBar(Control *ctrl);
  void changeControl(Control *ctrl);
};

#endif  // ABSTRACTION_H
