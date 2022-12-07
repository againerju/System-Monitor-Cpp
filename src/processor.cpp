#include <string>
#include <vector>
#include <iostream>
#include <thread>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;
using std::stoi;
using std::cout;

// Return the aggregate CPU utilization
float Processor::Utilization() { 

    // get number of cpu cores
    int numCpus = std::thread::hardware_concurrency();

    // read the uptime file
    std::string line, uptime, idletime; 
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename); 
    if (stream.is_open()) { 
        std::getline(stream, line); 
        std::istringstream linestream(line); 
        linestream >> uptime >> idletime; 
    }

    // compute the utilization
    float utilTime= (numCpus*std::stol(uptime) - std::stol(idletime)); 
    return utilTime/(numCpus*std::stol(uptime));
}