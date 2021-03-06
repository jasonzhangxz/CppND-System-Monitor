#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

/* System related data */
// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  long total, free;
  float mem_util;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          total = stoi(value);
        } else if (key == "MemFree:") {
          free = stoi(value);
          if ((total > free) && (total > 0)) {
            mem_util = (total - free) / (total * 1.0);
            return mem_util;
          }
        }
      }
    }
  }

  return mem_util;
}

// DONE: Read and return the system uptime, in seconds
long LinuxParser::UpTime() {
  string line, uptime_str, temp_str;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str >> temp_str;
  }
  if (uptime_str != "") {
    uptime = stol(uptime_str);
  }
  return uptime;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  long processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;
      if (key == "processes") {
        processes = (value != "" ? stoi(value) : 0);
      }
    }
  }
  return processes;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  long processes_running;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;
      if (key == "procs_running") {
        processes_running = (value != "" ? stoi(value) : 0);
      }
    }
  }
  return processes_running;
}

/* CPU related data */
// Done: Read and return the total number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<long> cpu_stat;
  long total;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;  // discard the first cpu
    if (value == "cpu") {
      while (linestream >> value) {
        cpu_stat.push_back((value != "" ? stol(value) : 0));
      }
    }
  }
  for (auto jiffy : cpu_stat) {
    total += jiffy;
  }
  return total;
}

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long utime;   //#14 CPU time spent in user code, measured in clock ticks
  long stime;   //#15 CPU time spent in kernel code, measured in clock ticks
  long cutime;  //#16 Amount of time that this process's waited-for children
                //have been scheduled in user mode, measured in clock ticks
  long cstime;  //#17 Amount of time that this process's waited-for children
                //have been scheduled in kernel mode, measured in clock ticks
  int counter = 14;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (counter > 0) {
      linestream >> value;
      counter--;
    }
    utime = (value != "" ? stol(value) : 0);
    linestream >> value;
    stime = (value != "" ? stol(value) : 0);
    linestream >> value;
    cutime = (value != "" ? stol(value) : 0);
    linestream >> value;
    cstime = (value != "" ? stol(value) : 0);
  }
  return utime + stime + cutime + cstime;
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<long> cpu_stat;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;  // discard the first cpu
    if (value == "cpu") {
      while (linestream >> value) {
        cpu_stat.push_back((value != "" ? stoi(value) : 0));
      }
    }
  }
  return cpu_stat[kUser_] + cpu_stat[kNice_] + cpu_stat[kSystem_] +
         cpu_stat[kIRQ_] + cpu_stat[kSoftIRQ_] + cpu_stat[kSteal_];
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<long> cpu_stat;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;  // discard the first cpu
    if (value == "cpu") {
      while (linestream >> value) {
        cpu_stat.push_back((value != "" ? stoi(value) : 0));
      }
    }
  }
  return cpu_stat[kIdle_] + cpu_stat[kIOwait_];
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_stat;
  string line, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;  // discard the first cpu
    if (value == "cpu") {
      while (linestream >> value) {
        cpu_stat.push_back(value);
      }
    }
  }
  return cpu_stat;
}

/* Processes */
// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmdline;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmdline);
  }

  return cmdline;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, key, value, ram_value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;
      if (key == "VmSize:") {
        ram_value = value;
        return ram_value;
      }
    }
  }
  return ram_value;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value, uid_string;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;
      if (key == "Uid:") {
        uid_string = value;
        return uid_string;
      }
    }
  }
  return uid_string;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, user, x, uid, username;
  string uid_string = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid;
      if (uid == uid_string) {
        username = user;
        return username;
      }
    }
  }
  return username;
}

// DONE: Read and return the uptime of a process, in clock ticks
long LinuxParser::UpTime(int pid) {
  string line, value;
  int counter = 22;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (counter > 0) {
      linestream >> value;
      counter--;
    }
  }
  return (value != "" ? stol(value) : 0);
}