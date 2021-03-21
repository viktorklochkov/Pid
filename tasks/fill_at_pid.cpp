#include "PidFiller.h"

#include "AnalysisTree/TaskManager.hpp"

using namespace AnalysisTree;


void fill_at_pid(const std::string& filelist, const std::string& pid_file){

  auto* man = TaskManager::GetInstance();
  man->SetOutputName("pid.root", "pTree");

  auto* pid_task = new PidFiller(pid_file, "pid_getter");
  pid_task->SetInputBranchNames({"VtxTracks", "TofHits"});

//  auto* mc_pid_task = new PidFillerMC();
//  mc_pid_task->SetInputBranchNames({"VtxTracks", "SimParticles"});


  man->AddTask(pid_task);

  man->Init({filelist}, {"rTree"});
  man->Run(-1);
  man->Finish();

}

int main(int argc, char **argv) {
  if(argc <= 2)
  {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./fill_at_pid filelist pid_file" << std::endl;
    return -1;
  }

  const std::string filelist = argv[1];
  const std::string pid_file = argv[2];

  fill_at_pid(filelist, pid_file);
  return 0;
}