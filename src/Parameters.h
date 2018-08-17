
#ifndef PidParameters_H
#define PidParameters_H 1

#include <vector>

#include "ParticleFit.h"

namespace Pid
{

class Parameters
{
public:
    
	/**   Default constructor   **/
	Parameters() ;
    
    void Parametrize(std::vector <ParticleFit> &particles);
    
    void SetParams( std::vector <double> &&x, 
                    std::vector <std::vector <double>> &&params,  
                    std::vector <std::vector <double>> &&params_errors)
    {
        params_ = params;
        params_errors_ = params_errors;
        x_ = x;
    }
    
//     void SetParticles( const std::vector <ParticleFit> &particles ) { particles_ = particles; }
    
    
private:

    std::vector <std::vector <double>> params_;    
    std::vector <std::vector <double>> params_errors_;    
    std::vector <double> x_;    
    
// 	ClassDef(Parameters,2);

};

}
#endif // PidParameters_H
