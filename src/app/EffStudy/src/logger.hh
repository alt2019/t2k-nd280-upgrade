#ifndef ___LOGGER
#define ___LOGGER 1

#include <string>
#include <sstream>
#include "G4ios.hh" // G4cout, G4endl

// extern std::stringstream _log_msg;
void log(std::string type, std::string file, std::string line, std::string func, std::string msg);
void log(std::string type, std::string file, std::string line, std::string func, std::stringstream& msg);

#endif