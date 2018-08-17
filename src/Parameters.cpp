#include "Parameters.h"

#include "TGraphErrors.h"
#include "TFile.h"


// ClassImp(Pid::Parameters)



namespace Pid
{

// -----   Default constructor   -------------------------------------------
Parameters::Parameters() 
{
}


void Parameters::Parametrize( std::vector <ParticleFit> &particles)
{
    std::vector <TGraphErrors> graphs;
    
    for (uint ipart=0; ipart<particles.size(); ++ipart)
    {
        const uint nvar = particles.at(ipart).GetNpar();
        for (uint ivar=0; ivar<nvar; ++ivar)
        {                
            std::vector <double> y;
            std::vector <double> dy;

            for (uint ibin=0; ibin<params_.size(); ++ibin)
            {
                const double par = params_.at(ibin).at( nvar*ipart + ivar );
                const double par_err = params_errors_.at(ibin).at( nvar*ipart + ivar );
                
//                 std::cout << par << " " <<  par_err << " " << std::endl;
                
                y.push_back(par);
                dy.push_back(par_err);

            }
            
            graphs.push_back( TGraphErrors( params_.size(), &(x_[0]), &(y[0]), 0, &(dy[0]) ) );
            graphs.back().Fit( &(particles.at(ipart).GetParametrizationFunction(ivar)), "Q" );
            graphs.back().Write( particles.at(ipart).GetParametrizationFunction(ivar).GetName() );
        }
        
        
    }
}


}
