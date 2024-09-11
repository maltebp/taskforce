#pragma once

#include <string>
#include <chrono>

namespace tf {

	class TimeEntry {
	public:

		std::string description;

		std::chrono::year_month_day date;

		std::chrono::minutes start_time;

		std::chrono::minutes end_time;

	};

}