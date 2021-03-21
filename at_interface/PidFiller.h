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


class PidFiller : public AnalysisTree::Task {
 public:

  PidFiller(const std::string& file, const std::string& getter);
  void Init() override;
  void Exec() override;
  void Finish() override {};
  ~PidFiller() override = default;

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

class PidFillerMC : public AnalysisTree::Task {
 public:

  PidFillerMC() = default;
  void Init() override;
  void Exec() override;
  void Finish() override {}
  ~PidFillerMC() override = default;


 protected:

  AnalysisTree::Particles* rec_particles_{nullptr};
  AnalysisTree::TrackDetector* tracks_{nullptr};
  AnalysisTree::Particles* mc_tracks_{nullptr};
  AnalysisTree::Matching* mc_match_{nullptr};

  std::string rec_particles_name_{"RecParticlesMcPid"};

};


#endif //ANALYSISTREECENTRALITY_SRC_PIDFILLER_H_
