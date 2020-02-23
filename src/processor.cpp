#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long prevTotal,prevIdle, total, idle;
    prevTotal = LinuxParser::Jiffies();
    prevIdle = LinuxParser::IdleJiffies();
    usleep(500); //sleep for 0.5s
    total = LinuxParser::Jiffies();
    idle = LinuxParser::IdleJiffies();
    return ((total - prevTotal)- (idle - prevIdle)) / (total - prevTotal);
}