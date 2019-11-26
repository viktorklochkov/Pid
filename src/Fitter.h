/** @file   Fitter.h
    @class  Pid::Fitter
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to fit 2D histograms
*/


#ifndef PidFitter_H
#define PidFitter_H 1

#include <vector>


#include "TString.h"
#include "TH2.h"
#include "TH1.h"

#include "ParticleFit.h"

namespace Pid
{

class Fitter
{

public:

//     BaseFitterHelper( std::vector <ParticleFit> &&particles ) : particles_(particles) {};
    Fitter() {};
    
    void Fit();
    TF1* ConstructFit1DFunction( float p );
    float Fit1D( std::unique_ptr <TH1D>& h, std::vector <double>& par, std::vector <double>& par_err, float p );
    void Clear();

    void AddParticle (ParticleFit& particle, uint id) { particles_.push_back(particle); particles_id_.push_back(id); }
    void SetHisto2D(std::unique_ptr <TH2D> histo2D) { histo2D_ = std::move(histo2D);}
    void SetRangeX(float min, float max) { minx_ = min, maxx_ = max; }
    void SetRangeY(float min, float max) { miny_ = min, maxy_ = max; }
    void SetOutputFileName(TString name) { outfilename_ = name; }
    
    ParticleFit GetParticle(uint i) const { return particles_.at(i); };
    ParticleFit GetParticleSpecie(uint i) const 
    { 
        return particles_.at( std::find(particles_id_.begin(), particles_id_.end(), i) - particles_id_.begin() ); 
    };
    
    void SetChi2Max (float chi2) { chi2_max_ = chi2; }
        
private:

    std::vector <ParticleFit> particles_;
    std::vector <uint> particles_id_;
    std::unique_ptr <TH2D> histo2D_{nullptr};
    
    TString outfilename_{"out.root"};
    
    float minx_{-1.};
    float maxx_{-1.};

    float miny_{-1.};
    float maxy_{-1.};
    
    float chi2_max_{100.};
    
//     ClassDef(BaseFitterHelper, 2);

};

}
#endif // PidFitter_H
