/** @file   Container.h
    @class  Pid::Container
    @author Viktor Klochkov (klochkov44@gmail.com)
    @author Ilya Selyuzhenkov (ilya.selyuzhenkov@gmail.com)
    @brief  Class to store PID information
*/

#ifndef CENTRALITY_CONTAINER_H
#define CENTRALITY_CONTAINER_H

#include <map>

namespace Pid {

class Container{

public:
    
    Container(){}
    
    void SetBayesianProbability( std::map<uint, float>&& prob ) { prob_ = prob; }
    void SetSigma( std::map<uint, float>&& sigma ) { sigma_ = sigma; }
    
    float GetBayesianProbability(uint num) const 
    { 
        auto find = prob_.find(num);        
        return find != prob_.end() ? find->second : -1;
    }
    
    float GetSigma(uint num) const 
    { 
        auto find = sigma_.find(num);        
        return find != sigma_.end() ? find->second : -1;
    }    
    
private:
    
    std::map <uint, float> prob_;
    std::map <uint, float> sigma_;
    
//     ClassDef(Container, 1);
    
};
}

#endif //CENTRALITY_CONTAINER_H
