#ifndef ANALYSISTREECENTRALITY_SRC_PIDFILLER_H_
#define ANALYSISTREECENTRALITY_SRC_PIDFILLER_H_

#include <string>
#include <iostream>
#include <memory>

#include "TFile.h"

#include "Getter.h"

#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/Matching.hpp"
#include "AnalysisTree/Task.hpp"

namespace AnalysisTree{

class PidFiller : public Task {
 public:

  PidFiller(const std::string& file, const std::string& getter);;
  void Init() override;
  void Exec() override;
  void Finish() override {};

 protected:

  std::string tracks_name_{"VtxTracks"};
  std::string tof_name_{"TofHits"};
  std::string rec_particles_name_{"RecParticles"};

  AnalysisTree::TrackDetector* tracks_{nullptr};
  AnalysisTree::HitDetector* tof_{nullptr};
  AnalysisTree::Matching* pid_match_{nullptr};
  AnalysisTree::Particles* rec_particles_{nullptr};

  ::Pid::Getter* getter_{nullptr};
};

class PidFillerMC : public Task {
 public:

  PidFillerMC() = default;
  void Init() override;
  void Exec() override;
  void Finish() override {}

 protected:

  AnalysisTree::Particles* rec_particles_{nullptr};
  AnalysisTree::TrackDetector* tracks_{nullptr};
  AnalysisTree::Particles* mc_tracks_{nullptr};
  AnalysisTree::Matching* mc_match_{nullptr};

};




}
#endif //ANALYSISTREECENTRALITY_SRC_PIDFILLER_H_
