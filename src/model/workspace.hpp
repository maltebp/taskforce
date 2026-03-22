#pragma once

#include <string>
#include <vector>

#include "serialization/object-serializer.dec.hpp"
#include "time-entry.hpp"


namespace tf {

	class Workspace {
	public:

		std::string name;

		std::vector<std::shared_ptr<TimeEntry>> time_entries;

	};

	// Serialization
	// -------------------------------------------------------------------------

	template<>
	struct ObjectSerializer<Workspace> {
		
		void write(WritableObject& writable_object, const Workspace& object_to_write);
		Workspace read(const ReadableObject& readable_object);

	};

}