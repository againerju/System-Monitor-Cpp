#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    float rest = 0.0;
    std::ostringstream elapsed_time;

    // get hours, minutes and seconds
    hours = int(times / 3600.);
    rest = (times % 3600) / 60.0;
    minutes = int(rest);
    seconds = int((rest-minutes) * 60);

    // string formatter
    elapsed_time << std::setfill('0') << std::setw(2) << std::right << hours << ":"
                 << std::setw(2) << std::right << minutes << ":"
                 << std::setw(2) << std::right << seconds;

    return elapsed_time.str();
}