#include "PidFiller.h"

#include "AnalysisTree/TaskManager.hpp"

using namespace AnalysisTree;

void fill_pid(const std::string& filelist, const std::string& output){

  auto* man = TaskManager::GetInstance();
  man->SetOutputName(output, "pTree");

  auto* pid_task = new PidFillerMC();
  pid_task->SetTracksName("VtxTracks");
  pid_task->SetMcParticlesName("SimParticles");
  pid_task->SetRecParticlesName("RecParticlesMcPid");

  man->AddTask(pid_task);

  man->Init({filelist}, {"rTree"});
  man->Run(-1);
  man->Finish();
}

int main(int argc, char **argv) {
  if(argc <= 1) {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./fill_mc_pid filelist" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filelist = argv[1];
  const std::string output_file = "pid.analysistree.root";

  fill_pid(filelist, output_file);
  return EXIT_SUCCESS;
}