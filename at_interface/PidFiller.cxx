#include <TaskManager.hpp>
#include "PidFiller.h"


PidFiller::PidFiller(const std::string& file, const std::string& getter) {

  std::cout << "PidFiller::PidFiller" << std::endl;
  auto pid_file = std::unique_ptr<TFile>( TFile::Open(file.c_str(), "read"));

  if ((!pid_file) || (pid_file->IsZombie())) {
    throw std::runtime_error("No file or file is zombie: " + file);
  }

  getter_ = pid_file->Get<Pid::Getter>(getter.c_str());
  if (getter_ == nullptr) {
    throw std::runtime_error("PID Getter is nullptr: " + getter);
  }
}

void PidFiller::Init() {

  auto* man = AnalysisTree::TaskManager::GetInstance();
  auto* chain = man->GetChain();

  tracks_ =ANALYSISTREE_UTILS_GET<AnalysisTree::TrackDetector*>(chain->GetPointerToBranch(tracks_name_));
  tof_ =ANALYSISTREE_UTILS_GET<AnalysisTree::HitDetector*>(chain->GetPointerToBranch(tof_name_));

  pid_match_ = chain->GetMatchPointers().find(config_->GetMatchName(tracks_name_, tof_name_))->second;
  mc_match_ = chain->GetMatchPointers().find(config_->GetMatchName(tracks_name_, mc_name_))->second;

  const auto& rec_tracks_conf = chain->GetConfiguration()->GetBranchConfig(tracks_name_);
  auto rec_part_config = rec_tracks_conf.Clone(rec_particles_name_, AnalysisTree::DetType::kParticle);

  std::vector<std::string> names{};
  for(const auto& pid : pid_codes_){
    names.push_back( "prob_" + pid.second);
  }

  rec_part_config.AddFields<float>(names, "probability to be proton, pion, kaon etc");

  man->AddBranch(rec_particles_name_, rec_particles_, rec_part_config);
  man->AddMatching(rec_particles_name_, mc_name_, mc_match_out_);
}

void PidFiller::Exec() {

  if(tracks_ == nullptr || pid_match_ == nullptr || tof_ == nullptr){
    std::cout << "PidFiller::Exec() input branch is nullptr! " << std::endl;
    exit(EXIT_FAILURE);
  }

  rec_particles_->ClearChannels();

  const auto* conf = AnalysisTree::TaskManager::GetInstance()->GetOutConfig();
  const auto& particles_config = conf->GetBranchConfig(rec_particles_name_);
  const int prob_id = particles_config.GetFieldId("prob_p");
  const int q_id = particles_config.GetFieldId("q");
  constexpr int Nspecies{4};

  for(const auto& track : *tracks_) {
    auto& particle = rec_particles_->AddChannel(particles_config);
    particle = AnalysisTree::Particle(track);
    particle.Init(particles_config);

    const float qp = track.GetP()*track.GetField<int>(q_id);
    const int tof_id = pid_match_->GetMatch(track.GetId());
    if ( tof_id >= 0) {
      const auto m2 = tof_->GetChannel(tof_id).GetField<float>(0);  //TODO
      particle.SetPid(track.GetField<int>(1) * getter_->GetPid(qp, m2, 0.5));

      const auto& prob = getter_->GetBayesianProbability(qp, m2);

      const int q = qp > 0 ? 1 : -1;
      for(size_t i=0; i<pid_codes_.size(); ++i){
        auto prob_i = prob.find(q*pid_codes_[i].first);
//        if(prob_i == prob.end()){
//          throw std::runtime_error("Pid code " + std::to_string(q*pid_codes[i].first) + " is not found in the getter!"); //TODO uncomment later
//        }
        particle.SetField(float(prob_i->second), prob_id+i);
      }

      for(int i=0; i<Nspecies; ++i){
      }
    }
    else{
      particle.SetPid(-1);
      for(int i=0; i<Nspecies; ++i){
        particle.SetField(-1.f, prob_id+1);
      }
    }
  }
  mc_match_out_->SetMatches(mc_match_->GetMatches(false), mc_match_->GetMatches(true));
}

void PidFillerMC::Init() {

  auto* man = AnalysisTree::TaskManager::GetInstance();
  auto* chain = man->GetChain();

  tracks_ =ANALYSISTREE_UTILS_GET<AnalysisTree::TrackDetector*>(chain->GetPointerToBranch(tracks_name_));
  mc_tracks_ =ANALYSISTREE_UTILS_GET<AnalysisTree::Particles *>(chain->GetPointerToBranch(mc_name_));
  mc_match_ = chain->GetMatchPointers().find(config_->GetMatchName(tracks_name_, mc_name_))->second;

  const auto& rec_tracks_conf = chain->GetConfiguration()->GetBranchConfig(tracks_name_);
  auto rec_part_config = rec_tracks_conf.Clone(rec_particles_name_, AnalysisTree::DetType::kParticle);
  man->AddBranch(rec_particles_name_, rec_particles_, rec_part_config);
}

void PidFillerMC::Exec() {

  if(tracks_ == nullptr || mc_match_ == nullptr || mc_tracks_ == nullptr){
    throw std::runtime_error("input branch is nullptr! ");
  }
  rec_particles_->ClearChannels();

  const auto* conf = AnalysisTree::TaskManager::GetInstance()->GetOutConfig();
  const auto& particles_config = conf->GetBranchConfig(rec_particles_name_);

  for (const auto& track : *tracks_) {
    auto& particle = rec_particles_->AddChannel(particles_config);

    particle = AnalysisTree::Particle(track);
    const int mc_id = mc_match_->GetMatch(track.GetId());
    if(mc_id >= 0) {
      particle.SetPid(mc_tracks_->GetChannel(mc_id).GetPid());
    }
    else{
      particle.SetPid(-999);
    }
  }
}
