#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma add namespace Pid;

#pragma link C++ class Pid::BaseGetter--;
#pragma link C++ class Pid::Getter+;
#pragma link C++ class Pid::CutGGetter+;

#pragma link C++ class Pid::ParticlePdf1DBase+;
#pragma link C++ class Pid::ParticlePdf1D+;
#pragma link C++ class Pid::ParticlePdf2DBase+;
#pragma link C++ class Pid::ParticlePdf2DBinned+;

#endif
