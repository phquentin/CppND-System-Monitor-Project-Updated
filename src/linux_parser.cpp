#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Reads and returns the Operating System (string)
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

// Reads and returns the kernel version (string)
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

// Reads and returns the system memory utilization in percent
float LinuxParser::MemoryUtilization() { 
  string line, key, unit;
  float value, MemTotal, MemFree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal:") {
          MemTotal = value;
        }
        if (key == "MemFree:"){
          MemFree = value;
        }
      }
    }
  }
  return (MemTotal-MemFree)/MemTotal;
}

// Reads and returns the system uptime
long LinuxParser::UpTime() { 
  long uptime, idle_process_time;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle_process_time;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Reads and returns CPU utilization times (vector<string>)
vector<float> LinuxParser::CpuUtilization() { 
  
  string line, key;
  float user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (key == "cpu") {
          vector<float> processor_times{user, nice, system, idle, iowait, irq, softirq, steal};
          return processor_times;
        }
      }
    }
  }
  }

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
}

//Reads and returns the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, unit, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) +kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "VmSize:") {
          return value;
        }
      }
    }
  }
}

// Reads and returns the user ID associated with a process
string LinuxParser::Uid(int pid) {  
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) +kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, key, value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
}

// TODO: Read and return the uptime of a process - not needed since already implemented in LinuxParser::ProcessCpuUtilization
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// Reads and returns the process times 
vector<float> LinuxParser::ProcessCpuUtilization(int pid){

  string line, pl1, pl2 ,pl3, pl4, pl5, pl6, pl7, pl8, pl9, pl10, pl11, pl12, pl13, pl18, pl19, pl20, pl21;
  float utime, stime, cutime, cstime, starttime;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> pl1 >> pl2 >> pl3 >> pl4 >> pl5 >> pl6 >> pl7 >> pl8 >> pl9 >> pl10 >> pl11 >> pl12 >> pl13 >> utime >> stime >> cutime >> cstime >> pl18 >> pl19 >> pl20 >> pl21 >> starttime;
    vector<float> process_times{utime, stime, cutime, cstime, starttime};
    return process_times;
  }

}
