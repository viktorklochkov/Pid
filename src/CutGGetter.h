#ifndef PID_SRC_CUTGGETTER_H_
#define PID_SRC_CUTGGETTER_H_

#include <TCutG.h>
#include <TText.h>
#include <TMultiGraph.h>

#include "Getter.h"

namespace Pid{
/**
 * @brief Pid getter based on graphical cut TCutG
 */
class CutGGetter : public TObject, public BaseGetter {

 public:
  void AddParticle(TCutG *cut, int pdgId);

  int GetPid(double var1, double var2, double) override;

  double GetWeight(double var1, double var2, int pid)  override {
    return 1.0 * (GetPid(var1, var2, 1) == pid);
  }

  std::map<int, double> GetWeights(double var1, double var2)  override;
  void Draw(Option_t *option = "") override;

 protected:
  std::map<int, std::vector<TCutG *>> species_{};

  ClassDefOverride(Pid::CutGGetter, 1)
};
}
#endif //PID_SRC_CUTGGETTER_H_
