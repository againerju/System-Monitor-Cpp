#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::stoul;
using std::vector;
using std::cout;

// constructor
Process::Process(int p) {
    SetPid(p);
    Process::cpu_usage_ = Process::CpuUtilization();
}

// TODO: Return this process's ID
int Process::Pid() {
    return Process::pid_;
}

void Process::SetPid(int p) {
    Process::pid_ = p;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    string elem;
    string key;
    string value;
    unsigned long int total_time;
    unsigned long int seconds;
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(Process::Pid()) + LinuxParser::kStatFilename);
    vector<string> timers;
    if (filestream.is_open()) {
        while (std::getline(filestream, elem, ' ')) {
            timers.push_back(elem);
        }
    }
    unsigned long int utime = std::stoul(timers[13]);
    unsigned long int stime = std::stoul(timers[14]);
    total_time = utime + stime;

    unsigned long int cutime = std::stoul(timers[15]);
    unsigned long int cstime = std::stoul(timers[16]);
    total_time = total_time + cutime + cstime;

    unsigned long int hertz = sysconf(_SC_CLK_TCK);
    unsigned long int starttime = std::stoul(timers[21]);
    seconds = LinuxParser::UpTime(Process::Pid()) - (starttime / hertz);

    float cpu_usage;
    cpu_usage = (total_time / hertz) / seconds;

    return cpu_usage;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(Process::Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() {     
    string ram = LinuxParser::Ram(Process::Pid());
        return std::to_string(int(std::stoi(ram) / 1024.));
    }

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(Process::Pid());
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(Process::Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  // sort by cpu usage => greater than 
  return (cpu_usage_ > a.cpu_usage_) ; 
}