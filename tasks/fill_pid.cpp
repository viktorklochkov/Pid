#include "PidFiller.hpp"

#include "AnalysisTree/TaskManager.hpp"

using namespace AnalysisTree;

void fill_pid(const std::string& filelist, const std::string& pid_file, const std::string& output){

  auto* man = TaskManager::GetInstance();
  man->SetOutputName(output, "aTree");
  man->SetOutputMode(eBranchWriteMode::kCopyTree);

  auto* pid_task = new PidFiller(pid_file, "pid_getter");
//  pid_task->SetTracksName("VtxTracks");
//  pid_task->SetTofName("TofHits");
//  pid_task->SetMcParticlesName("SimParticles");
//  pid_task->SetPidCodes({{2212, "p"}, {211, "pi"}, {321, "K"}, {1, "bg"}});

  man->AddTask(pid_task);

  man->Init({filelist}, {"rTree"});
  man->Run(10);
  man->Finish();
}

int main(int argc, char **argv) {
  if(argc <= 2) {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./fill_pid filelist pid_file" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string filelist = argv[1];
  const std::string pid_file = argv[2];
  const std::string output_file = "pid.analysistree.root";

  fill_pid(filelist, pid_file, output_file);
  return EXIT_SUCCESS;
}