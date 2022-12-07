#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::stoul;
using std::vector;
using std::cout;

// Constructor
Process::Process(int p) {
    SetPid(p);
    Process::cpu_usage_ = Process::CpuUtilization();
}

// Return this process's ID
int Process::Pid() {
    return Process::pid_;
}

void Process::SetPid(int p) {
    Process::pid_ = p;
}

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    
    int pid = Pid();
    string line, value;
    std::vector<std::string> utilValues;
    float utime, stime,cutime,starttime,cstime;

    std::string path=LinuxParser::kProcDirectory +std::to_string(pid)+LinuxParser::kStatFilename;
    std::ifstream stream(path);
    int idx=0;

    if (stream.is_open()) {
        std::getline(stream,line);
        std::istringstream linestream(line);

        while(idx<22){
            linestream>>value;
            utilValues.push_back(value);
            idx++;
        }
        
        utime=std::stof(utilValues[13]);
        stime=std::stof(utilValues[14]);
        cutime=std::stof(utilValues[15]);
        cstime=std::stof(utilValues[16]);
        starttime=std::stof(utilValues[21]);
        double hertz=sysconf(_SC_CLK_TCK);
        int numCpus=std::thread::hardware_concurrency();
        double uptime=numCpus*LinuxParser::UpTime();

        float total_time = utime + stime;
        total_time = total_time + cutime + cstime;

        float seconds = uptime - (starttime / hertz);
        return   ((total_time / hertz) / seconds);

    }

    return 0;
}

// Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(Process::Pid());
}

// Return this process's memory utilization
string Process::Ram() {     
    string ram = LinuxParser::Ram(Process::Pid());
        return std::to_string(int(std::stoi(ram) / 1024.));
    }

// Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(Process::Pid());
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(Process::Pid());
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  // sort by cpu usage => greater than 
  return (cpu_usage_ > a.cpu_usage_) ; 
}