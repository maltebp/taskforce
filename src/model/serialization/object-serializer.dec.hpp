#pragma once

#include "serialization/writable-object.fwd.hpp"
#include "serialization/readable-object.fwd.hpp"

namespace tf {

    // TODO: Comment here
	template<typename T>
	struct ObjectSerializer {

		// TODO: False type here

		void write(WritableObject& object, const T& value);

		// TODO: Figure out how to do proper move here
		T read(const ReadableObject& object);

	private:

	};

	// TODO: Predefine some common types

}