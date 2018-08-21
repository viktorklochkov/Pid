/** @file   ParticleFit.h
    @author Viktor Klochkov (klochkov44@gmail.com)
    @date   August 2018
    @brief  Class to store fit resuls for particle specie
*/

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
	ParticleFit(int type) : particle_type_(type) {} ;
    
    const TF1& GetFunction() const { return function_; }
    
    std::vector <double> GetFunctionParams(float p) const
    {
        std::vector <double> params;
        const uint npar = function_.GetNpar();        

        if ( parametrization_.size() != npar )
            exit(0);        

        if (!isfitted_)
            return params;

        for ( uint i=0; i<npar; ++i )
        {
            params.push_back( parametrization_.at(i).Eval(p) );
        }
        return params;
    }
    
    float Eval(float p, float m2)
    {
        if (p>maxx_ || p<minx_) return 0.;
        
        const uint npar = function_.GetNpar();        
        if ( parametrization_.size() != npar )
            exit(0);        
        
        function_.SetParameters( &(GetFunctionParams(p)[0]) );
        return function_.Eval(m2);
    }
    
    void SetParametrization(const std::vector <TF1> &parametrization) { parametrization_ = parametrization; }
    void SetFitFunction(const TF1 &function) { function_ = function; }
    
    void SetRange(float min, float max) { minx_ = min, maxx_ = max; }
    
    uint GetNpar() const {return function_.GetNpar();}
    
    TF1& GetParametrizationFunction(int ipar) { return parametrization_.at(ipar); }
    
    void SetIsFitted(bool is=true) { isfitted_=is; }
    void SetIsFixed( const std::vector <bool>& is ) { isfixed_=is; }
    
    bool GetIsFixed(uint ipar) const 
    { 
        if (ipar>=isfixed_.size()) 
            return false;
        return isfixed_.at(ipar); 
    }
    
    
private:

    TF1 function_;
    std::vector <TF1> parametrization_{};
    std::vector <bool> isfixed_{};
    
    float minx_{-1.};
    float maxx_{-1.};
    
    int npoints_{-1};
    int particle_type_{-1};
    
    bool isfitted_{false};
        
        
// 	ClassDef(ParticleFit,2);

};

}
#endif // PidParticleFit_H
