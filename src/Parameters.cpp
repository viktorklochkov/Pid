#include "Parameters.h"

// ClassImp(Pid::Parameters)



namespace Pid
{

// -----   Default constructor   -------------------------------------------
Parameters::Parameters() 
{
}


void Parameters::Parametrize()
{
    std::vector <TGraphErrors> graphs;
    
    for (uint ipart=0; ipart<particles_.size(); ++ipart)
    {
        const uint nvar = particles_.at(ipart).GetNpar();
        for (uint ivar=0; ivar<nvar; ++ivar)
        {                
            for (uint ibin=0; ibin<params.size(); ++ibin)
            {
                std::cout << params_.at(ibin).at( nvar*ipart + ivar ) << " " ;
                std::cout << params_errors_.at(ibin).at( nvar*ipart + ivar ) << " " ;
                std::cout << std::endl;

            }
        }
    }    
    
    
}


}
