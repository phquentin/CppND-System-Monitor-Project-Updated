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
    float sys_clock_ticks = sysconf(_SC_CLK_TCK);
    float total_time = ProcessCPUTimes[0] + ProcessCPUTimes[1] + ProcessCPUTimes[2] + ProcessCPUTimes[3];
    float seconds = LinuxParser::UpTime() - (ProcessCPUTimes[4]/sys_clock_ticks);
    float cpu_usage = 100 * ((total_time / sys_clock_ticks) / seconds);

    return cpu_usage; 
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// Returns the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
