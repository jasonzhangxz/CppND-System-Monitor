#include "linux_parser.h"
#include "ncurses_display.h"
#include "process.h"
#include "system.h"

int main() {
  // int time = LinuxParser::UpTime();
  // std::string uid = LinuxParser::Uid(3038);
  //    Process process = Process(1551);
  //    int time = process.UpTime();
  //    std::string ram = process.Ram();
  //    Processor processor = Processor();
  //    float util = processor.Utilization();
  System system;
  NCursesDisplay::Display(system);
}