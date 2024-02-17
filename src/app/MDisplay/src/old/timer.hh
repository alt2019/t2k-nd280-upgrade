/**
 * @file timer.hh
 *
 * @brief Implementation of timer
 *
 * @creation_date     { --.--.2021 }
 * @modification_date ( --.--.2021 )
 *
 * @author Alexandr Shvartsman
 * Contact: shvartsman.as@phystech.edu, shvartsman@inr.ru
 *
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

struct Timer
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
	std::chrono::time_point<std::chrono::high_resolution_clock> stop_time;
	bool is_timed = false;

	Timer()
	{
		start_time = std::chrono::high_resolution_clock::now();
		is_timed = true;
	}

	void stop(const char * msg)
	{
		stop_time = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>( stop_time - start_time ).count();
		std::cout << msg << ": execution done in " << duration << " mks" << std::endl;
		is_timed = false;
	}
};


std::string getCurrentTime()
{
	time_t now = time(0);
	tm *ltm = localtime(&now);

	int YYYY = 1900 + ltm->tm_year;
	int MM = 1 + ltm->tm_mon;
	int DD = ltm->tm_mday;

	int H = ltm->tm_hour;
	int M = ltm->tm_min;
	int S = ltm->tm_sec;

	std::string curr_date = to_string(DD) + "." + to_string(MM) + "." + to_string(YYYY);
	std::string curr_time = to_string(H) + "." + to_string(M) + "." + to_string(S);
	curr_time = curr_date + "-" + curr_time;

	return curr_time;
}

#endif