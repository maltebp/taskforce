#pragma once

#include <string>
#include <chrono>

namespace tf {

	class TimeEntry {
	public:

		std::string description;

		std::chrono::year_month_day date;

		std::chrono::duration<double> start_time;

		std::chrono::duration<double> end_time;

	};

}