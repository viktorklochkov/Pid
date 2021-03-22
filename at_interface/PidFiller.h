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

  void SetTracksName(const std::string& tracks_name) {
    tracks_name_ = tracks_name;
    in_branches_.insert(tracks_name);
  }

  void SetTofName(const std::string& tof_name) {
    tof_name_ = tof_name;
    in_branches_.insert(tof_name);
  }

  void SetRecParticlesName(const std::string& rec_particles_name) {
    rec_particles_name_ = rec_particles_name;
  }

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

  void SetTracksName(const std::string& tracks_name) {
    tracks_name_ = tracks_name;
    in_branches_.insert(tracks_name);
  }

  void SetMcName(const std::string& mc_name) {
    mc_name_ = mc_name;
    in_branches_.insert(mc_name);
  }

  void SetRecParticlesName(const std::string& rec_particles_name) {
    rec_particles_name_ = rec_particles_name;
  }

 protected:

  AnalysisTree::Particles* rec_particles_{nullptr};
  AnalysisTree::TrackDetector* tracks_{nullptr};
  AnalysisTree::Particles* mc_tracks_{nullptr};
  AnalysisTree::Matching* mc_match_{nullptr};

  std::string tracks_name_{"VtxTracks"};
  std::string mc_name_{"SimParticles"};
  std::string rec_particles_name_{"RecParticlesMcPid"};

};


#endif //ANALYSISTREECENTRALITY_SRC_PIDFILLER_H_
