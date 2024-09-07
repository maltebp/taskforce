#pragma once

#include <string>
#include <vector>

#include "time-entry.hpp"


namespace tf {

	class Workspace {
	public:

		std::string name;

		std::vector<TimeEntry> time_entries;

	};

}