#pragma once

#include <iostream>
#include <functional>

#include "core/result.fwd.hpp"

#include "serialization/readable-object.fwd.hpp"
#include "serialization/writable-object.fwd.hpp"


namespace tf {

    class Serializer {
	public:

		template<typename T>
		[[nodiscard]] Result<void> write(std::ostream& stream, const T& value);

		template<typename T>
		Result<T> read(std::istream& stream);

	protected:

		virtual void write_internal(
			std::ostream& stream, 
			std::function<void(WritableObject& root_object)> write_callback
		) = 0;

		/**
		 * @return 
		 * Reference to the last object read from this method. I.e. if this method is called again 
		 * the reference of the old call now points to the object read from the new call. In other
		 * words, this value should not be stored.
		 */
		[[nodiscard]] virtual const ReadableObject& read_internal(std::istream& stream) = 0;

		virtual void reset() = 0;

	};

}