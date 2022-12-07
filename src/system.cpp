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

// Constructor
System::System() {
  SetKernel(); 
  SetOperatingSystem();
}

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    vector<int> pids = LinuxParser::Pids();
    processes_ = {};

    for (const auto& p : pids)  {
        processes_.emplace_back(Process(p));
    }
    // sort processes: max to min 
    std::sort(processes_.begin(), processes_.end());  
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// Set the kernel name
void System::SetKernel() {
    kernel_ = LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() { 
    return LinuxParser::MemoryUtilization(); 
}

// Return the operating system name
std::string System::OperatingSystem() { 
    return ops_; 
}

// Set the name of the operating system
void System::SetOperatingSystem() {
    ops_ = LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() { 
    return LinuxParser::RunningProcesses(); 
}

// Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() {
    return LinuxParser::UpTime();
}