#include "PidFiller.hpp"

using namespace AnalysisTree;

PidFiller::PidFiller(const std::string& file, const std::string& getter) {
  std::cout << "PidFiller::PidFiller" << std::endl;
  auto pid_file = std::unique_ptr<TFile>(TFile::Open(file.c_str(), "read"));

  if ((!pid_file) || (pid_file->IsZombie())) {
    throw std::runtime_error("No file or file is zombie: " + file);
  }
  getter_ = pid_file->Get<Pid::Getter>(getter.c_str());
  if (getter_ == nullptr) {
    throw std::runtime_error("PID Getter is nullptr: " + getter);
  }
}

int PidFiller::signum(int x) const {
  if(x>0)
    return 1;
  else if (x==0)
    return 0;
  else
    return -1;
}

void PidFiller::Init() {
  auto man = TaskManager::GetInstance();
  auto chain = man->GetChain();
  rec_tracks_ = chain->GetBranchObject(rec_tracks_name_);
  tof_hits_ = chain->GetBranchObject(tof_hits_name_);
  pid_match_ = chain->GetMatchPointers().find({rec_tracks_name_ + "2" + tof_hits_name_})->second;

  in_branches_.emplace(rec_tracks_name_);
  in_branches_.emplace(tof_hits_name_);

  auto conf = rec_tracks_.GetConfig().Clone(out_branch_name_, AnalysisTree::DetType::kParticle);

  std::vector<std::string> names{};
  for (const auto& pid : pid_codes_) {
    names.push_back("prob_" + pid.second);
  }
  conf.AddFields<float>(names, "probability to be proton, pion, kaon etc");

  ana_tracks_ = Branch(conf);
  ana_tracks_.SetMutable();
  ana_tracks_.Freeze();

  rec_tracks_.Freeze();

  man->AddBranch(&ana_tracks_);

  int i{0};
  auto match_br = {"SimParticles", "RichRings", "TofHits", "TrdTracks"};
  out_matches_.assign(match_br.size(), nullptr);

  for(const auto& br : match_br){
    in_matches_.emplace_back(chain->GetMatchPointers().find({rec_tracks_name_ + "2" + br})->second);
    man->AddMatching(out_branch_name_, br, out_matches_.at(i));
    i++;
  }
  
  qp_tof_field_ = tof_hits_.GetField("qp_tof");
  q_field_ = rec_tracks_.GetField("q");
  mass2_field_ = tof_hits_.GetField("mass2");
  pid_field_ = ana_tracks_.GetField("pid");
  
  for (const auto& pid : pid_codes_) {
    prob_field_.push_back(ana_tracks_.GetField("prob_" + pid.second));
  }  
}

void PidFiller::Exec() {
  ana_tracks_.ClearChannels();

  for (size_t i = 0; i < rec_tracks_.size(); ++i) {
    const auto& track = rec_tracks_[i];
    auto particle = ana_tracks_.NewChannel();
    particle.CopyContent(track);
    auto q = track[q_field_];

    auto hit_id = pid_match_->GetMatch(i);
    if (hit_id >= 0) {
      const auto& tof_hit = tof_hits_[hit_id];
      auto pq = tof_hit[qp_tof_field_];
      auto m2 = tof_hit[mass2_field_];

      auto pid = getter_->GetPid(pq, m2, 0.5);
      if(pid == 1 && q < 0) {
        pid = -1;
      }
      
      particle.SetValue(pid_field_, pid);
      auto prob = getter_->GetBayesianProbability(pq, m2);

      particle.Print();

      int specie{0};
      for (const auto& pdg : pid_codes_) {
        particle.SetValue(prob_field_.at(specie++), prob[pdg.first*signum(q)]);  // Think what to do in case of electrons and muons
      }
    } else {
      int specie{0};
      for (const auto& pdg : pid_codes_) {
        particle.SetValue(prob_field_.at(specie++), -1.f);
      }
      particle.SetValue(pid_field_, 2*signum(q));
    }
  }
  int i{0};
  for (auto& match : out_matches_){
    auto m1 = in_matches_[i]->GetMatches(false);
    auto m2 = in_matches_[i]->GetMatches(true);
    match->SetMatches(m1, m2);
    i++;
  }
}
