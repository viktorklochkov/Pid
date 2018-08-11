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

    Fitter() {};
    
    void AddParticle (const ParticleFit& particle) { particles_.push_back(particle); }

    void SetHisto2D(std::unique_ptr <TH2D> histo2D) { histo2D_ = std::move(histo2D);}
    void SetRange(float min, float max) { minx_ = min, maxx_ = max; }
    TF1* ConstructFit1DFunction();
    
    void Parametrize2D( std::vector <std::vector <double>> &params, std::vector <std::vector <double>> &params_errors );

    void Fit();
    void Fit1D( std::unique_ptr <TH1D>& h, std::vector <double>& par, std::vector <double>& par_err );
private:

    std::vector <ParticleFit> particles_;
    std::unique_ptr <TH2D> histo2D_;
    
    float minx_{-1.};
    float maxx_{-1.};
    
//     ClassDef(Fitter, 2);

};

}
#endif // PidFitter_H
