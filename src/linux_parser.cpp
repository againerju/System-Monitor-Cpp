#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoul;
using std::string;
using std::to_string;
using std::vector;
using std::cout;

// Read operating system from the filesystem
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
  filestream.close();
  return value;
}

// Read kernel name from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// Get PIDs
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string unit;
  float memTotal = 0.0;
  float memFree = 0.0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit) {
        if (key == "MemTotal") {
          memTotal = std::stof(value);
        } else if (key == "MemFree") {
          memFree = std::stof(value);

        } else {
        }
      }
    }
  }
  filestream.close();

  // compute and return memory utilization
  float memUsage = memTotal - memFree;
  return memUsage/memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string time;
  string idle;
  long uptime = 0.0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time >> idle;
    uptime = std::stol(time);
    return uptime;
  }
  filestream.close();
  return uptime;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  vector<string> cpu_util;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
          cpu_util = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
        }
      }
    }
  }
  filestream.close();
  return cpu_util;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value = 0;
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
  filestream.close();
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  int value = 0;
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
  filestream.close();
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string cmd;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
      std::replace(line.begin(), line.end(), '\n', ' ');
      cmd += line;
      cmd += " ";
    }
  }
  filestream.close();
  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  string uid;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
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
  filestream.close();

  return nullptr;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string value;
  string other;
  string other_two;
  string uid;
  string user;
  // get user id
  string query_uid = LinuxParser::Uid(pid);

  // get user name
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> other >> uid >> other_two) {
        if (query_uid == uid) {
          user = key;
          break;
        }
      }
    }
  }
  filestream.close();
  return user; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string elem;
  long uptime = 0;
  long int starttime;
  float clk_ticks = float(sysconf(_SC_CLK_TCK));
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatFilename);
  vector<string> timers;
  if (filestream.is_open()) {
      while (std::getline(filestream, elem, ' ')) {
          timers.push_back(elem);
      }
  }
  filestream.close();

  starttime = std::stoul(timers[21]) / clk_ticks;
  uptime = LinuxParser::UpTime() - (starttime); 
  return uptime;
}
