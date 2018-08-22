#include "ParticleFit.h"

// ClassImp(Pid::ParticleFit)

namespace Pid
{

/// Default constructor   -------------------------------------------
ParticleFit::ParticleFit() 
{
}


/**
* Getter for vector of parameters for a ParticleFit
* @param p track momentum
* @return vector of parameters
*/
std::vector <double>&& ParticleFit::GetFunctionParams(float p) const
{
    std::vector <double> params;
    const uint npar = function_.GetNpar();        

    if ( parametrization_.size() != npar )
        exit(0);        

    if (!isfitted_)
        return std::move(params);

    for ( uint i=0; i<npar; ++i )
    {
        params.push_back( parametrization_.at(i).Eval(p) );
    }
    return std::move(params);
}

/**
* Similar to TF1::Eval
* @param p track momentum
* @param m2 track mass square
* @return vector of parameters
*/
float ParticleFit::Eval(float p, float m2)
{
    if (p>maxx_ || p<minx_) return 0.;
    
    const uint npar = function_.GetNpar();        
    if ( parametrization_.size() != npar )
        exit(0);        
    
    function_.SetParameters( &(GetFunctionParams(p)[0]) );
    return function_.Eval(m2);
}


}
