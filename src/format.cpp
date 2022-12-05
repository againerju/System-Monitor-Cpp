#include <string>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) {
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    float rest = 0.0;
    std::string elapsed_time;
    hours = int(times / 3600.);
    rest = (times % 3600) / 60.0;
    minutes = int(rest);
    seconds = int((rest-minutes) * 60);

    elapsed_time = std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    return elapsed_time;
}