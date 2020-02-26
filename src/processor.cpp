#include "processor.h"
#include <unistd.h>
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  long prevTotal, prevActive, prevIdle, total, active, idle;
  prevTotal = LinuxParser::Jiffies();
  prevActive = LinuxParser::ActiveJiffies();
  prevIdle = LinuxParser::IdleJiffies();
  usleep(50000);  // sleep for 50ms
  total = LinuxParser::Jiffies();
  active = LinuxParser::ActiveJiffies();
  idle = LinuxParser::IdleJiffies();
  return (1.0 * (active - prevActive)) /
         (active + idle - prevActive - prevIdle);
  //    return (1.0*(active - prevActive)) / (total - prevTotal);
  //    return (1.0*active ) / total;
}