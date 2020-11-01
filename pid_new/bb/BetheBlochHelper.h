//
// Created by eugene on 26/11/2019.
//

#ifndef PID_BB_BETHEBLOCHHELPER_H_
#define PID_BB_BETHEBLOCHHELPER_H_

#include <functional>

#include "BetheBlochShine.h"

#include <pid_new/core/PdgHelper.h>

typedef std::function<double (double)> BetheBlochFunc_t;

class BetheBlochHelper {

 public:
  static BetheBlochFunc_t makeBBForMass(double mass, BetheBlochFunc_t bb_src = BetheBlochAntoniMod);

  static BetheBlochFunc_t makeBBForPdg(Pdg_t pdg, BetheBlochFunc_t bb_src = BetheBlochAntoniMod);


};

#endif //PID_BB_BETHEBLOCHHELPER_H_
