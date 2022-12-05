#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  SetKernel(); 
  SetOperatingSystem();
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    vector<int> pids = LinuxParser::Pids();
    processes_ = {};

    for (const auto& p : pids)  {
        processes_.push_back(Process(p));
    }
    
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

void System::SetKernel() {
    kernel_ = LinuxParser::Kernel();
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
    return ops_; 
}

void System::SetOperatingSystem() {
    ops_ = LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() {
    //long uptime = 1000.0;
    //return uptime;
    return LinuxParser::UpTime();
}