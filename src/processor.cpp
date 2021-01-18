#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>
#include <numeric>

using std::vector;
using std::string;

// Returns the aggregate CPU utilization
float Processor::Utilization() { 
    
    const vector<float> processor_times = LinuxParser::CpuUtilization();
    const float total = std::accumulate(processor_times.begin(),processor_times.end(),0.0);
    const float idled = processor_times[3] + processor_times[4];
    const float cpu_percentage = (total - idled)/total;

    return cpu_percentage; 
    }
