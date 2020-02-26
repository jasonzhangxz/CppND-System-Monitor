#include <unistd.h> // to get sysconf(_SC_CLK_TCK)
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() {
    return pid_;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    long process_ticks = LinuxParser::ActiveJiffies(pid_);
    float elapse_sec = LinuxParser::UpTime() - LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK);
    float cpu_util_ = 100.0*(1.0*process_ticks/sysconf(_SC_CLK_TCK))/elapse_sec;
    return cpu_util_;
}

// DONE: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(pid_);
}

// DONE: Return this process's memory utilization
string Process::Ram() {
    string ram_string;
    ram_string = LinuxParser::Ram(pid_);
    if (ram_string != "") {
        ram_ = std::stoi(ram_string) / 1024; //convert from KB to MB
    }
    return std::to_string(ram_);
}

// DONE: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(pid_);
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(pid_)/sysconf(_SC_CLK_TCK);
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    if (cpu_util_ < a.cpu_util_){
        return true;
    } else {
        return false;
    }
}