
#include "ColorDefinitions.hh"

std::string cprint(std::string target, std::string color)
{
	std::string result = color;
	result += target;
	result += RESET;
	return result;
}

std::string cprint(float target, std::string color)
{
	std::string result = color;
	result += std::to_string(target);
	result += RESET;
	return result;
}

std::string print_green(std::string str)
{
	return GREEN + str + RESET;
}

std::string print_yellow(std::string str)
{
	return YELLOW + str + RESET;
}


/*********************************************************
 * Converts double value to RGB color scheme
 * 
 * @standard c++17
 *
 * @creation_date     { --.--.2021 }
 * @modification_date { 20.03.2021 }
 *
 * @param[double]  val  { double value to convert ro color }
 * @return[std::string] { color }
 * 
 *********************************************************/
std::string double2color(double val) {
	// std::string color;
	int ival = int(val * 1e6);
	// float r = ((val >> 16) & 0xff) / 255.0f;
	// float g = ((val >>  8) & 0xff) / 255.0f;
	// float b = ((val      ) & 0xff) / 255.0f;
    int r = ((ival >> 16) & 0xff);
    int g = ((ival >>  8) & 0xff);
    int b = ((ival      ) & 0xff);

    std::string color = "rgb(" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b) + ")";
	return color;
}


/*********************************************************
 * Converts double value to HSL color scheme
 * 
 * @standard c++17
 *
 * @creation_date     { --.--.2021 }
 * @modification_date { 15.06.2021 }
 *
 * @param[double]  val  { double value to convert ro color in range [0, 1] }
 * @return[std::string] { color }
 * 
 *********************************************************/
std::string double2color_v2(double val) {
	float h = (1.0 - val) * 230.0;
	std::string color = "hsl(" + std::to_string(h) + ", 100%, 50%)";
	return color;
}


/*********************************************************
 * @standard c++17
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date { 20.03.2021 }
 *
 * @param[LoggingType]  type      { type of log message }
 * @param[const char *] title     { title of message }
 * @param[std::string]  message   { message content }
 * @param[std::string]  file_path { file to write logs }
 * @return[] {  }
 * 
 *********************************************************/
void logging::WriteToLog(
  logging::LoggingType type,
  const char * title,
  std::string message,
  [[maybe_unused]] std::string file_path)
{
  switch (type)
  {
    case LT_INFO   : std::cerr << BOLDGREEN << __DATE__ << " " << __TIME__  << " [INFO] "
    							 << BLUE << title << RESET << message << std::endl; break;
    case LT_WARNING: std::cerr << YELLOW << __DATE__ << " " << __TIME__  << " [WARN] "
    							 << BLUE << title << RESET << message << std::endl; break;
    case LT_ERROR  : std::cerr << RED << __DATE__ << " " << __TIME__  << " [ERROR] "
    							 << BLUE << title << RESET << message << std::endl; break;
    case LT_DEBUG  : std::cerr << CYAN << __DATE__ << " " << __TIME__  << " [DEBUG] "
    							 << BLUE << title << RESET << message << std::endl; break;
  }
}