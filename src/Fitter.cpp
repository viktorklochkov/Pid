#include <iostream>
#include "Fitter.h"

// ClassImp(Pid::Fitter);

namespace Pid
{

void Fitter::Fit()
{
    std::cout << histo2D_->GetNbinsX() << std::endl;

    const uint hnbinsX = histo2D_->GetNbinsX() ;
    const float hminX = histo2D_->GetXaxis()->GetXmin() ;
    const float hmaxX = histo2D_->GetXaxis()->GetXmax() ;

    const uint firstbin = (minx_-hminX)/(hmaxX-hminX)*hnbinsX ;
    const uint lastbin = (maxx_-hminX)/(hmaxX-hminX)*hnbinsX ;
    
    for (uint ibin=firstbin; ibin<lastbin; ++ibin)
    {
        std::cout << ibin << std::endl;
        std::unique_ptr <TH1D> h1fit { histo2D_->ProjectionY( Form("py_%d", ibin), ibin, ibin) };
        
        const std::vector <double> &par = Fit1D(h1fit);
        
        std::cout << par.size() << " " << par.at(0) << " " << par.at(1) << " " << par.at(2) << std::endl;
        
    }
}
    
const std::vector <double> Fitter::Fit1D( std::unique_ptr <TH1D>& h )
{
//     auto f = particles_.at(0).GetFunction(0.);
    auto f = ConstructFit1DFunction(); //particles_.at(0).GetFunction(0.);
    
    h->Fit( f, "" );
    
//     const double* params = f.GetParameters();
    const std::vector <double> ret (f->GetParameters(), f->GetParameters() + f->GetNpar() );
    return ret;
}

TF1* Fitter::ConstructFit1DFunction()
{
//     const TF1 *FitFunc1DHisto  = &(particles_.at(0).GetFunction(0.));
    TF1 *temp = new TF1(particles_.at(0).GetFunction(0.).GetName(), particles_.at(0).GetFunction(0.).GetExpFormula());
    TString SFitFunc = Form("%s", temp->GetName() );
    
    for (uint i=1; i<particles_.size(); ++i)
    {
        temp = new TF1(particles_.at(i).GetFunction(0.).GetName(), particles_.at(i).GetFunction(0.).GetExpFormula());
        SFitFunc += Form( "+%s", temp->GetName() );
    }
    
    TF1* ret = new TF1("fit1D", SFitFunc, minx_, maxx_ );
    return ret;
    
}


}
