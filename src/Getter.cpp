#include "Getter.h"

ClassImp(Pid::Getter)
ClassImp(Pid::CutGGetter)

namespace Pid
{


/**
* Constructs fit function as a sum of individual particle species. Parameters are also propagated
* @param p track momentum (or x-axis value, in general)
* @param m2 square mass for TOF (or y-axis value, in general)
* @return map with probabilities for all particle species
*/
std::map<uint, float> Getter::GetBayesianProbability(float p, float m2)
{
    std::map<uint, float> prob{};

    if (p>maxx_ || p<minx_) 
        return std::move(prob);
    
    float sum{0.};
    for (auto &specie : species_)
    {
        const float iprob = specie.second.Eval(p, m2);
        sum += iprob;
        prob[specie.first] = iprob;
    }
//     std::cout << sum << std::endl;
    if (sum < 1.) return std::move(prob); // no particles in this (p, m2) piont
    
    for (auto &iprob : prob)
    {
        iprob.second /= sum;
    }

    return prob;
}

}
