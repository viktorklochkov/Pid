/** @file   Container.h
    @class  Pid::Container
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to store PID information
*/

#ifndef PID_CONTAINER_H
#define PID_CONTAINER_H

#include <map>

namespace Pid {

class Container {

 public:

  Container() = default;

  void SetBayesianProbability(std::map<uint, double> &&prob) { prob_ = prob; }
  void SetSigma(std::map<uint, double> &&sigma) { sigma_ = sigma; }

  double GetBayesianProbability(uint num) const {
    auto find = prob_.find(num);
    return find != prob_.end() ? find->second : -1;
  }

  double GetSigma(uint num) const {
    auto find = sigma_.find(num);
    return find != sigma_.end() ? find->second : -1;
  }

 private:

  std::map<uint, double> prob_;
  std::map<uint, double> sigma_;

//     ClassDef(Container, 1);

};
}

#endif //PID_CONTAINER_H
