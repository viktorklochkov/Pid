#include <iostream>
#include "Fitter.h"

#include "TGraphErrors.h"
#include "TFile.h"
#include "TMath.h"

#include "Parameters.h"


// ClassImp(Pid::BaseFitterHelper);

namespace Pid
{
/**
* Main function. Fitting TH2D bin-by-bin
*/
void Fitter::Fit()
{
    const uint firstbin = histo2D_->GetXaxis()->FindBin(minx_);
    const uint lastbin = histo2D_->GetXaxis()->FindBin(maxx_);
    
    std::vector <std::vector <double>> params;
    std::vector <std::vector <double>> params_errors;
    std::vector <double> x;

    std::unique_ptr <TFile> f{ TFile::Open( outfilename_, "recreate") };

    for (uint ibin=firstbin; ibin<lastbin; ++ibin)
    {
        std::unique_ptr <TH1D> h1fit { histo2D_->ProjectionY( Form("py_%d", ibin), ibin, ibin) };
        
        std::vector <double> par;
        std::vector <double> par_err;
        
        const float mom = histo2D_->GetXaxis()->GetBinCenter(ibin);
        const float chi2 = Fit1D(h1fit, par, par_err, mom);
        
        std::cout << mom << "  " << chi2 << std::endl;
        
        if (chi2 > chi2_max_) continue;
        
        params.push_back(par);
        params_errors.push_back(par_err);
        x.push_back( mom );
    }
    
    Parameters p;
    p.SetParams( std::move(x), std::move(params), std::move(params_errors) );
//     p.SetParticles();
    p.Parametrize(particles_);
    
    f->Close();

}

/**
* Constructs fit function as a sum of individual particle species. Parameters are also propagated
* @param h pointer to input histo
* @param par output: fit parameters 
* @param par_err output: fit parameters erorrs
* @param p track momentum
* @return chi2/NDF of the fit
*/
float Fitter::Fit1D( std::unique_ptr <TH1D>& h, std::vector <double>& par, std::vector <double>& par_err, float p)
{
    auto f = ConstructFit1DFunction(p); //particles_.at(0).GetFunction(0.);
        
    h->Fit( f, "Q,L", "", miny_, maxy_ );

    par = std::vector<double> (f->GetParameters(), f->GetParameters() + f->GetNpar() );
    par_err = std::vector<double>  (f->GetParErrors(), f->GetParErrors() + f->GetNpar() );    
    
    h->Write( Form("h_%f", p) );
    
    return f->GetChisquare()/f->GetNDF();
}


/**
* Constructs fit function as a sum of individual particle species. Parameters are also propagated
* @param p track momentum
* @return pointer to TF1 function
*/
TF1* Fitter::ConstructFit1DFunction(float p)
{
    TF1 *temp{nullptr}; 
    TString sumname{""};
    std::vector <double> par{};
    
    uint iparticle{0};
    for (auto const& particle : particles_)
    {
        const TString name = particle.GetFunction().GetName();
        
        temp = (TF1*)particle.GetFunction().Clone(name); 
        iparticle == 0 ? sumname=name : sumname += "+" + name;        
        std::vector <double> par_i = particle.GetFunctionParams(p);
        
        par.insert(par.end(), par_i.begin(), par_i.end());
        
        iparticle++;
    }

    TF1* f = new TF1("fit1D", sumname, miny_, maxy_ );   
    f->SetParameters( &(par[0]) );

    uint iparam_all{0};
    for (auto const& particle : particles_)
    {
        for (uint iparam=0; iparam<particle.GetNpar(); ++iparam, ++iparam_all)
        {
            double parmin{-1.};
            double parmax{-1.};
            particle.GetFunction().GetParLimits(iparam, parmin, parmax);
            f->SetParLimits( iparam_all, parmin, parmax );
            
            if ( particle.GetIsFixed(iparam) )
                f->FixParameter( iparam_all, par.at(iparam_all) );
        }
        
    }
//     for (auto ipar : par)
//         std::cout << ipar << " ";
//     std::cout << std::endl;
//     std::cout << f->GetName() << " " << f->GetExpFormula() << std::endl;
    
    delete temp;
    
    return f;
}
/**
* Clear everything
*/
void Fitter::Clear()
{
    particles_.clear();
    particles_id_.clear();
    histo2D_.reset();
    minx_=-1.;
    maxx_=-1.;
    miny_=-1.;
    maxy_=-1.;
    chi2_max_=100.;
    outfilename_="out.root";
}


}
