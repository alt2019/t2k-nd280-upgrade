
#include "logger.hh"

void log(std::string type, std::string file, std::string line, std::string func, std::string msg)
{
  G4cout << "\033[32m[" << type <<  "]\033[33m" << file << "\033[0m:\033[36m"
                                  << line << "\033[0m:\033[35m"
                                  << func << "\033[0m: "
       << msg << G4endl;

  // _log_msg.str("");
}

void log(std::string type, std::string file, std::string line, std::string func, std::stringstream& msg)
{
  G4cout << "\033[32m[" << type <<  "]\033[33m" << file << "\033[0m:\033[36m"
                                  << line << "\033[0m:\033[35m"
                                  << func << "\033[0m: "
       << msg.str() << G4endl;

  msg.str("");
}