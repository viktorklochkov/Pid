#include <iostream>
#include "Fitter.h"

#include "TGraphErrors.h"

// ClassImp(Pid::Fitter);

namespace Pid
{

void Fitter::Fit()
{
    const uint firstbin = histo2D_->GetXaxis()->FindBin(minx_);
    const uint lastbin = histo2D_->GetXaxis()->FindBin(maxx_);
    
    std::vector <std::vector <double>> params;
    std::vector <std::vector <double>> params_errors;
    
    for (uint ibin=firstbin; ibin<lastbin; ++ibin)
    {
        std::unique_ptr <TH1D> h1fit { histo2D_->ProjectionY( Form("py_%d", ibin), ibin, ibin) };
//         const std::vector <double> &par = Fit1D(h1fit);
        
        std::vector <double> par;
        std::vector <double> par_err;
        
        Fit1D(h1fit, par, par_err);
        
        params.push_back(par);
        params_errors.push_back(par_err);

        //         std::cout << params.back().size() << " " << params.back().at(0) << " " << params.back().at(1) << " " << params.back().at(2) << std::endl;
    }
    Parametrize2D(params, params_errors);
}
    
void Fitter::Fit1D( std::unique_ptr <TH1D>& h, std::vector <double>& par, std::vector <double>& par_err  )
{
    auto f = ConstructFit1DFunction(); //particles_.at(0).GetFunction(0.);
    h->Fit( f, "Q" );

    par = std::vector<double> (f->GetParameters(), f->GetParameters() + f->GetNpar() );
    par_err = std::vector<double>  (f->GetParErrors(), f->GetParErrors() + f->GetNpar() );    
//     return ret;
}

TF1* Fitter::ConstructFit1DFunction(  )
{
    TString name = particles_.at(0).GetFunction(0.).GetName();
    TF1 *temp = (TF1*)particles_.at(0).GetFunction(0.).Clone(name); 
    
    TString sumname = Form("%s", temp->GetName() );
    
    for (auto particle : particles_)
    {
        name = particle.GetFunction(0.).GetName();
        temp = (TF1*)particle.GetFunction(0.).Clone(name); 
        sumname += "+" + name;
    }
    TF1* f = new TF1("fit1D", sumname, minx_, maxx_ );    
    f->SetParameters(1000.,0.,1.);
//     std::cout << f->GetName() << " " << f->GetExpFormula() << std::endl;
    
    return f;
}

void Fitter::Parametrize2D( std::vector <std::vector <double>> &params, 
                            std::vector <std::vector <double>> &params_errors )
{
    std::vector <TGraphErrors> graphs;
    
    for (uint ipart=0; ipart<particles_.size(); ++ipart)
    {
        const uint nvar = particles_.at(ipart).GetNpar();
        for (uint ivar=0; ivar<nvar; ++ivar)
        {                
            for (uint ibin=0; ibin<params.size(); ++ibin)
            {
                std::cout << params.at(ibin).at( nvar*ipart + ivar ) << " " ;
                std::cout << params_errors.at(ibin).at( nvar*ipart + ivar ) << " " ;
                std::cout << std::endl;

            }
        }
    }
        
}





}
