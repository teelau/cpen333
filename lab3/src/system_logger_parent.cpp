#include <cpen333/process/subprocess.h>
#include <cpen333/process/mutex.h>
#include <string>

int main() {

  // shared inter-process mutex
  cpen333::process::mutex mutex("system_logger");

  // list of child processes
  std::vector<cpen333::process::subprocess> processes;

  const int nprocesses = 20;
  const int nmessages = 100;
  const std::string logfile = "system_log.txt";

  for (int i=0; i<nprocesses; ++i) {
    std::vector<std::string> cmd;

    //=================================================
    //  RUN CHILD PROCESSES (system_logger_child)
    //      Process arguments:
    //          - a name for identification in the log
    //          - a log filename to append to
    //          - a number of messages to append
    //=================================================
    cmd.push_back("./bin/system_logger_child.exe");  // relative command path
    // other arguments...
    // cmd.push_back(...);
    cmd.push_back("process #"+std::to_string(i));
    cmd.push_back(logfile);
    cmd.push_back(std::to_string(nmessages));
    std::cout << cmd.size();
    processes.push_back(cmd);

  }

  // wait for processes to finish
  for (auto& process : processes) {
    process.join();
  }

  // unlink mutex since we and all child processes are done with it
  mutex.unlink();

  // pause, waiting for keyboard input
  cpen333::pause();
  return 0;
}
