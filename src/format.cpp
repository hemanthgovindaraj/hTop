#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    std::ostringstream timestream;
    timestream << std::right << std::setfill('0') << std::setw(2) << std::to_string(int(seconds/3600)) << ":" << std::right << std::setfill('0') <<  std::setw(2) << std::to_string(int((seconds/60)%60)) << ":" << std::right << std::setfill('0') << std::setw(2) << std::to_string(int(seconds%60));
    return timestream.str();
}