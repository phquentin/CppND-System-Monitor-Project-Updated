#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Returns this process's ID
int Process::Pid() { return pid; }

// Returns this process's CPU utilization
float Process::CpuUtilization() { 
    vector<float> ProcessCPUTimes = LinuxParser::ProcessCpuUtilization(pid);
    const float sys_clock_ticks = sysconf(_SC_CLK_TCK);
    const float total_time = ProcessCPUTimes[0] + ProcessCPUTimes[1] + ProcessCPUTimes[2] + ProcessCPUTimes[3];
    const float seconds = LinuxParser::UpTime() - (ProcessCPUTimes[4]/sys_clock_ticks);
    const float cpu_usage = ((total_time / sys_clock_ticks) / seconds);

    return cpu_usage; 
}

// Returns the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// Returns this process's memory utilization
string Process::Ram() { 
    int rami = stoi(LinuxParser::Ram(pid));
    rami = rami / 1000;
    const string ram = to_string(rami);

    return ram; 
}

// Returns the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// Returns the age of this process (in seconds)
long int Process::UpTime() { 
    const vector<float> ProcessCPUTimes = LinuxParser::ProcessCpuUtilization(pid);
    const long uptime = LinuxParser::UpTime() - (ProcessCPUTimes[4]/sysconf(_SC_CLK_TCK));

    return uptime; 
}

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return a.cpu_utilization < cpu_utilization;
}
