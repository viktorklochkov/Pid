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

  auto rec_part_config = AnalysisTree::BranchConfig(rec_particles_name_, AnalysisTree::DetType::kParticle);

//  rec_particles_ = new AnalysisTree::Particles;

  man->AddBranch(rec_particles_name_, rec_particles_, rec_part_config);
}

void PidFiller::Exec() {

  if(tracks_ == nullptr || pid_match_ == nullptr || tof_ == nullptr){
    std::cout << "PidFiller::Exec() input branch is nullptr! " << std::endl;
    exit(EXIT_FAILURE);
  }

  rec_particles_->ClearChannels();
  auto* man = AnalysisTree::TaskManager::GetInstance();
  const auto* conf = man->GetOutConfig();
  const auto& particles_config = conf->GetBranchConfig(rec_particles_name_);

  for(const auto& track : *tracks_) {
    auto& particle = rec_particles_->AddChannel(particles_config);

    particle.SetMomentum3(track.GetMomentum3());
    const float mom = track.GetP();
    const int tof_id = pid_match_->GetMatch(track.GetId());
    if ( tof_id >= 0) {
      const auto m2 = tof_->GetChannel(tof_id).GetField<float>(0);  //TODO
      particle.SetPid(track.GetField<int>(1) * getter_->GetPid(mom, m2, 0.8));
    }
    else{
      particle.SetPid(-1);
    }
  }
}

void PidFillerMC::Init() {

//  tracks_ = (AnalysisTree::TrackDetector*) branches.find(in_branches_[0])->second;
//  mc_tracks_ = (AnalysisTree::Particles*) branches.find(in_branches_[1])->second;
//  mc_match_ = (AnalysisTree::Matching*) branches.find(config_->GetMatchName(in_branches_[0], in_branches_[1]))->second;
//
//  auto rec_part_config = BranchConfig(out_branch_, AnalysisTree::DetType::kParticle);
//  out_config_->AddBranchConfig(rec_part_config);
//  rec_particles_ = new Particles;
//  out_tree_ -> Branch(out_branch_.c_str(), "AnalysisTree::Particles", &rec_particles_);
//  branches.emplace(out_branch_, rec_particles_);
}

void PidFillerMC::Exec() {

//  if(tracks_ == nullptr || mc_match_ == nullptr || mc_tracks_ == nullptr){
//    std::cout << "PidFillerMC::Exec() input branch is nullptr! " << std::endl;
//    exit(1);
//  }
//  rec_particles_->ClearChannels();
//  const auto& particles_config = out_config_->GetBranchConfig(out_branch_);
//
//  for (const auto& track : *tracks_->GetChannels()) {
//    auto* particle = rec_particles_->AddChannel();
//    particle->Init(particles_config);
//    particle->SetMomentum3(track.GetMomentum3());
//    const int mc_id = mc_match_->GetMatch(track.GetId());
//    if(mc_id >= 0) {
//      particle->SetPid(mc_tracks_->GetChannel(mc_id).GetPid());
//    }
//    else{
//      particle->SetPid(-999);
//    }
//  }
}
