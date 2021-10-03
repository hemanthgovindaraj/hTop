#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{ 
    return std::to_string(int(seconds/3600)) + ":" + std::to_string(int((seconds/60)%60)) + ":" + std::to_string(int(seconds%60));
}