#include "CutGGetter.h"
ClassImp(Pid::CutGGetter)

namespace Pid{

void CutGGetter::AddParticle(TCutG* cut, int pdgId) {
  if (cut) {
    auto insert_result = species_.insert({pdgId, {cut}});
    if (!insert_result.second) {
      (*insert_result.first).second.push_back(cut);
    }
    return;
  }

  throw std::logic_error("empty TCutG object");
}

int CutGGetter::GetPid(double var1, double var2, double) {

  for (const auto &specie : species_) {
    int pdgId = specie.first;
    auto specieCuts = specie.second;

    for (auto cut : specieCuts) {
      if (cut->IsInside(var1, var2)) {
        return pdgId;
      }
    }
  }

  return -1;
}

std::map<int, double> CutGGetter::GetWeights(double var1, double var2) {
  std::map<int, double> result;

  for (const auto &specie : species_) {
    result.insert({specie.first, GetWeight(var1, var2, specie.first)});
  }

  return std::map<int, double>();
}

void CutGGetter::Draw(Option_t* option) {
  TObject::Draw(option);

  TMultiGraph mg("mg", "");
  TText pdgLabel;

  for (const auto &specie : species_) {
    auto specieCuts = specie.second;
    for (auto cut : specieCuts) mg.Add(cut);
  }

  mg.DrawClone(option);

  for (const auto &specie : species_) {
    int pdgId = specie.first;
    auto specieCuts = specie.second;

    double xc, yc;
    for (auto cut : specieCuts) {
      cut->Center(xc, yc);
      pdgLabel.DrawText(xc, yc, Form("%d", pdgId));
    }
  }
}
}