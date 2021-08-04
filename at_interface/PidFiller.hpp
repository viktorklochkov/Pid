#ifndef PID_AT_INTERFACE_PIDFILLER_HPP_
#define PID_AT_INTERFACE_PIDFILLER_HPP_

#include "Getter.h"

#include "AnalysisTree/Task.hpp"
#include "AnalysisTree/TaskManager.hpp"

class PidFiller : public AnalysisTree::Task {

 public:
  PidFiller(const std::string& file, const std::string& getter);
  ~PidFiller() override { delete getter_; }

  void Init() override;
  void Exec() override;
  void Finish() override {}

 protected:
  AnalysisTree::Branch rec_tracks_;
  AnalysisTree::Branch tof_hits_;
  AnalysisTree::Branch ana_tracks_;
  AnalysisTree::Matching* pid_match_{nullptr};

  std::string rec_tracks_name_{"VtxTracks"};
  std::string tof_hits_name_{"TofHits"};
  std::string out_branch_name_{"RecParticles"};

  Pid::Getter* getter_{nullptr};
  std::vector<std::pair<long long, std::string>> pid_codes_{
      {{2212, "p"}, {211, "pi"}, {321, "K"}, {1, "bg"}}};
};

#endif// PID_AT_INTERFACE_PIDFILLER_HPP_
