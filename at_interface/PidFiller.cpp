#include "PidFiller.hpp"

using namespace AnalysisTree;

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
  auto man = TaskManager::GetInstance();
  auto chain = man->GetChain();
  rec_tracks_ = chain->GetBranch(rec_tracks_name_);
  tof_hits_ = chain->GetBranch(tof_hits_name_);
  pid_match_ = chain->GetMatchPointers().find({rec_tracks_name_+"2"+tof_hits_name_})->second;

  in_branches_.emplace(rec_tracks_name_);
  in_branches_.emplace(tof_hits_name_);

  auto conf = rec_tracks_.GetConfig().Clone(out_branch_name_, AnalysisTree::DetType::kParticle);

  std::vector<std::string> names{};
  for(const auto& pid : pid_codes_){
    names.push_back( "prob_" + pid.second);
  }
  conf.AddFields<float>(names, "probability to be proton, pion, kaon etc");

  ana_tracks_ = Branch(conf);
  ana_tracks_.SetMutable();
  man->AddBranch(&ana_tracks_);
}

void PidFiller::Exec() {
  ana_tracks_.ClearChannels();

  auto field_p = rec_tracks_.GetField("p");
  auto field_m2 = tof_hits_.GetField("mass2");
  auto field_pid = ana_tracks_.GetField("pid");
  std::vector<Field> fields_prob{};
  for(const auto& pid : pid_codes_){
    fields_prob.push_back(ana_tracks_.GetField("prob_" + pid.second));
  }

  for(int i=0; i<rec_tracks_.size(); ++i){
    const auto& track = rec_tracks_[i];

    auto particle = ana_tracks_.NewChannel();
    auto raw_track = *(track.Data<Track>());
    *(particle.Data<Particle>()) = Particle(raw_track);

    auto hit_id = pid_match_->GetMatch(i);
    if(hit_id >= 0){
      const auto& tof_hit = tof_hits_[hit_id];
      auto p = track[field_p];
      auto m2 = tof_hit[field_m2];

      auto pid = getter_->GetPid(p, m2, 0.5);
      particle.SetValue(field_pid, pid);
      auto prob = getter_->GetBayesianProbability(p, m2);

      int specie{0};
      for(const auto& pdg : pid_codes_){
        particle.SetValue(fields_prob[specie++], prob[pdg.first]);
      }
    }
    else{
      particle.SetValue(field_pid, -1);
    }
  }
//    mc_match_out_->SetMatches(mc_match_->GetMatches(false), mc_match_->GetMatches(true));

}
