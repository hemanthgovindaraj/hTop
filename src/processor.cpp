#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    float util;
    long totalJiffies{LinuxParser::Jiffies()};
    long idleJiffies{LinuxParser::IdleJiffies()};
    
    util = ( totalJiffies - idleJiffies )/float(totalJiffies);
    
    return util;
}