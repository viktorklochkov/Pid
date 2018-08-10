
#ifndef PidParticleFit_H
#define PidParticleFit_H 1

#include <vector>
#include <array>

#include "TF1.h"


namespace Pid
{

class ParticleFit
{
public:
    
	/**   Default constructor   **/
	ParticleFit() ;
    
    const TF1& GetFunction(float p)
    {
        const uint npar = function_.GetNpar();
        
        if ( parametrization_.size() != npar )
            exit(0);
        if (!isfitted_)
            return function_;
        
        std::vector <double> params;
        for ( uint i=0; i<npar; ++i )
        {
            params.push_back( parametrization_.at(i).Eval(p) );
        }
        function_.SetParameters(&(params[0]));
        return function_;
    }
    
    void SetParametrization(const std::vector <TF1> &parametrization) { parametrization_ = parametrization; }
    void SetFitFunction(const TF1 &function) { function_ = function; }
    
    void SetRange(float min, float max) { minx_ = min, maxx_ = max; }
    
    
private:

    TF1 function_;
    std::vector <TF1> parametrization_;
    
    float minx_{-1.};
    float maxx_{-1.};
    
    int npoints_{-1};
    
    bool isfitted_{false};
        
        
// 	ClassDef(ParticleFit,2);

};

}
#endif // PidParticleFit_H
