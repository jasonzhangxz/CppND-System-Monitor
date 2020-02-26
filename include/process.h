#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <unistd.h> // to get sysconf(_SC_CLK_TCK)

#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid):pid_(pid){} //constructor and initialize pid
  int Pid();                               // DONE: See src/process.cpp
  std::string User();                      // DONE: See src/process.cpp
  std::string Command();                   // DONE: See src/process.cpp
  float CpuUtilization();                  // DONE: See src/process.cpp
  std::string Ram();                       // DONE: See src/process.cpp
  long int UpTime();                       // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp

 private:
    int pid_{0};
    float cpu_util_{0};
    int ram_{0};
};

#endif