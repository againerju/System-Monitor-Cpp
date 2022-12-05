#include <string>
#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;
using std::stoi;
using std::cout;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> cpu_util = LinuxParser::CpuUtilization();
    int user = std::stoi(cpu_util[0]);
    int nice = std::stoi(cpu_util[1]);
    int system = std::stoi(cpu_util[2]);
    int idle = std::stoi(cpu_util[3]);
    int iowait = std::stoi(cpu_util[4]);
    int irq = std::stoi(cpu_util[5]);
    int softirq = std::stoi(cpu_util[6]);
    int steal = std::stoi(cpu_util[7]);
         
    float Idle = idle + iowait;
    float NonIdle = user + nice + system + irq + softirq + steal;
    float Total = Idle + NonIdle;
    float Util = NonIdle / Total;

    return Util;
}