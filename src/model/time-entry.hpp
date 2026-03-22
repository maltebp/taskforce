#pragma once

#include <string>
#include <chrono>

#include "serialization/object-serializer.dec.hpp"

namespace tf {

	class TimeEntry {
	public:

		std::string description;

		std::chrono::year_month_day date;

		std::chrono::minutes start_time;

		std::chrono::minutes end_time;

	};

	// Serialization
	// -------------------------------------------------------------------------

	template<>
	struct ObjectSerializer<TimeEntry> {
		
		void write(WritableObject& writable_object, const TimeEntry& object_to_write);
		
		TimeEntry read(const ReadableObject& readable_object);

	};

}