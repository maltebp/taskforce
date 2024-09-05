#pragma once

#include <string>
#include <chrono>

namespace tf {

	class TimeEntry {
	public:

		std::string description;

		std::time_t date;

		std::time_t start_time;

		std::time_t end_time;

	};

}