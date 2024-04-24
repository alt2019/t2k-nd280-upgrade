/**
 * @file ColorDefinitions.hh
 *
 * @brief Declarations of color variables and functions
 *
 * @creation_date     { 24.03.2024 }
 * @modification_date ( 24.03.2024 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef COLOR_DEFINITIONS_H
#define COLOR_DEFINITIONS_H

#include <iostream>
#include <string>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

std::string cprint(std::string target, std::string color);
std::string cprint(float target, std::string color);


std::string print_green(std::string str);
std::string print_yellow(std::string str);



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
std::string double2color(double val);


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
std::string double2color_v2(double val);


/***************************************************************************************************************
 * Custom logger
 * 
 * @standard c++17
 *
 * @creation_date     { 20.03.2021 }
 * @modification_date { 20.03.2021 }
 *
 ***************************************************************************************************************/
namespace logging {
  enum LoggingType
  {
    LT_INFO,
    LT_WARNING,
    LT_ERROR,
    LT_DEBUG
  };

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
  void WriteToLog(LoggingType type, const char * title, std::string message, [[maybe_unused]] std::string file_path = "");
}

#endif