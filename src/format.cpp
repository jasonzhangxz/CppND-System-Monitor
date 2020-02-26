#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hour, min, sec;
  string HH, MM, SS;
  sec = seconds % 60;
  min = (seconds / 60) % 60;
  hour = seconds / 3600;
  HH = (hour > 9 ? to_string(hour) : "0" + to_string(hour));
  MM = (min > 9 ? to_string(min) : "0" + to_string(min));
  SS = (sec > 9 ? to_string(sec) : "0" + to_string(sec));
  return HH + ":" + MM + ":" + SS;
}