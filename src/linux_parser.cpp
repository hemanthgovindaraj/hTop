#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string line;
  double memTotal{1},memFree{0};
  std::ifstream mem_file(kProcDirectory+kMeminfoFilename);
  if(mem_file.is_open())
  {
    std::string key{""};
    double value{0};
    while(std::getline(mem_file,line))
    {
      std::istringstream linestream(line);
      linestream>> key >> value;
      if(key=="MemTotal:")
      {
        memTotal = value;
      }
      else if(key=="MemFree:")
      {
        memFree = value;
        break;
      }
    }
  }
  double util{0.0};
  util = (memTotal - memFree)/memTotal;
  return util;
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime{0}; 
  std::string line{""};
  std::ifstream stat(kProcDirectory+kUptimeFilename);
  if(stat.is_open())
  {
    std::getline(stat,line);
    std::istringstream linestream(line);
    linestream>>uptime;
  }
  return uptime;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  long totalJiffies;

  totalJiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();

  return totalJiffies; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{ 
  long activeJiffies;
  std::ifstream statFile{kProcDirectory+std::to_string(pid)+kStatFilename};

  if(statFile.is_open())
  {
    std::string line;
    std::getline(statFile,line);
    activeJiffies = std::stol(LinuxParser::FindData(line,14)) + std::stol(LinuxParser::FindData(line,15));
  }
   
  return activeJiffies;
  
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{ 
  long activeJiffies;
  std::vector<string> util;
  
  util = LinuxParser::CpuUtilization();
  activeJiffies = stol(util.at(0)) + stol(util.at(1)) + stol(util.at(2)) + stol(util.at(5)) + stol(util.at(6)) + stol(util.at(7));
  
  return activeJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  long idleJiffies;
  std::vector<string> util;
  
  util = LinuxParser::CpuUtilization();
  idleJiffies = stol(util.at(3)) + stol(util.at(4));
  
  return idleJiffies;   
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  std::vector<std::string> util{};
  std::string line,cpu; 
  std::ifstream stat{kProcDirectory+kStatFilename};

  if(stat.is_open())
  {
    std::getline(stat,line);
    std::istringstream linestream(line);
    linestream>>cpu;
    while(linestream)
    {
      std::string jiffies;
      linestream>>jiffies;
      util.push_back(jiffies);
    }
  }
  return util;  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  std::string line,key;
  long procTotal; 
  std::ifstream  procinfo{kProcDirectory+kStatFilename};

  if(procinfo.is_open())
  {
    while(std::getline(procinfo,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> procTotal;
      if(key=="processes")
      {
        return procTotal;
      }
    }
  }
  return procTotal; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::string line,key;
  long procRunning;
  std::ifstream  procinfo{kProcDirectory+kStatFilename};

  if(procinfo.is_open())
  {
    while(std::getline(procinfo,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> procRunning;
      if(key=="procs_running")
      {
        return procRunning;
      }
    }
  }
  return procRunning; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  std::string cmd;
  std::ifstream cmdline{kProcDirectory+std::to_string(pid)+kCmdlineFilename};
  
  if(cmdline.is_open())
  {
    std::getline(cmdline,cmd);
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  std::string ram,key,line;
  long ramSize;
  std::ifstream statusfile{kProcDirectory+std::to_string(pid)+kStatusFilename};
  
  if(statusfile.is_open())
  {
    while(std::getline(statusfile,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> ram;
      if(key=="VmSize:")
      {
        break;
      }
    }
  }
  ramSize = stol(ram)/1024;
  return std::to_string(ramSize);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{ 
  std::string uid,key,line;
  std::ifstream statusfile{kProcDirectory+std::to_string(pid)+kStatusFilename};
  
  if(statusfile.is_open())
  {
    while(std::getline(statusfile,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if(key=="Uid:")
      {
        break;
      }
    }
  }
  
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) 
{ 
  std::string user,line,x,uid_x;
  std::string uid{LinuxParser::Uid(pid)};

  std::ifstream passwdfile{kPasswordPath};

  if(passwdfile.is_open())
  {
    while(std::getline(passwdfile,line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream>>user>>x>>uid_x;
      if(uid_x==uid)
      {
        return user;
      }
    }
  }
  
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) 
{ 
  long upTime;
  std::string line,time_up;
  std::ifstream statFile{kProcDirectory+std::to_string(pid)+kStatFilename};

  if(statFile.is_open())
  {
    std::getline(statFile,line);
    time_up = LinuxParser::FindData(line,22);
  }
  upTime = std::stol(time_up)/sysconf(_SC_CLK_TCK);
  upTime = LinuxParser::UpTime() - upTime;

  return upTime;
}

std::string LinuxParser::FindData(std::string const& line, int const& index)
{
  std::string data;
  std::istringstream linestream(line);
  
  for(int i=0;i<=(index-1);++i)
  {linestream >> data;}
  
  if(!data.empty())
  {
    return data;
  }
  else
  {
    return "0";
  }
}
