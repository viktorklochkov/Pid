#ifndef PID_AT_INTERFACE_PIDFILLER_HPP_
#define PID_AT_INTERFACE_PIDFILLER_HPP_

#include "Getter.h"
#include "Constants.h"

#include "AnalysisTree/Task.hpp"
#include "AnalysisTree/TaskManager.hpp"

class PidFiller : public AnalysisTree::Task {

 public:
  PidFiller(const std::string& file, const std::string& getter);
  ~PidFiller() override { delete getter_; }

  void Init() override;
  void Exec() override;
  void Finish() override {
    auto* man = AnalysisTree::TaskManager::GetInstance();
//    man->RemoveBranch(rec_tracks_name_);
  }

 protected:
  int signum(int x) const;
   
  AnalysisTree::Branch rec_tracks_;
  AnalysisTree::Branch tof_hits_;
  AnalysisTree::Branch ana_tracks_;
  AnalysisTree::Matching* pid_match_{nullptr};
  
  AnalysisTree::Field qp_tof_field_;
  AnalysisTree::Field q_field_;
  AnalysisTree::Field mass2_field_;
  AnalysisTree::Field pid_field_;
  std::vector<AnalysisTree::Field> prob_field_{};

  std::vector <AnalysisTree::Matching*> in_matches_{};
  std::vector <AnalysisTree::Matching*> out_matches_{};

  std::string rec_tracks_name_{"VtxTracks"};
  std::string tof_hits_name_{"TofHits"};
  std::string out_branch_name_{"RecParticles"};

  Pid::Getter* getter_{nullptr};
  std::vector<std::pair<long long, std::string>> pid_codes_{
       {PidParticles::kProton, "p"},
       {PidParticles::kPionPos, "pi"},
       {PidParticles::kKaonPos, "K"},
       {PidParticles::kDeutron, "d"},
       {PidParticles::kBgPos, "bg"}};
       
};

#endif// PID_AT_INTERFACE_PIDFILLER_HPP_
