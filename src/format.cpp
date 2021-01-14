#include <string>
#include <chrono>

#include "format.h"

using std::string;

// transforms seconds (long) into HH:MM:SS (String)
string Format::ElapsedTime(long seconds) { 

 int hours = seconds/3600;
 int rest = seconds%3600;
 int minutes = rest/60;
 int remaining_seconds = rest%60;

 return Convert_to_String_format(hours)+ ":" + Convert_to_String_format(minutes)+ ":" + Convert_to_String_format(remaining_seconds); 
}

string Format::Convert_to_String_format(int time){
 if(time < 10){
  return "0" + std::to_string(time);
 }
 else{
  return std::to_string(time);
 }

}
