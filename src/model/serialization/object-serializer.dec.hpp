#pragma once

#include "core/false_type.hpp"

#include "serialization/writable-object.fwd.hpp"
#include "serialization/readable-object.fwd.hpp"

namespace tf {

    /**
     * @brief Define how your custom type is to be serialized by implementing \ref 
	 * ObjectSerializer::write and \ref ObjectSerializer::read.
     */
	template<typename T>
	struct ObjectSerializer {

		static_assert(tf::FalseType<T>::value, "No definition found of ObjectSerializer<T> for this type");

		void write(WritableObject& object, const T& value);

		T read(const ReadableObject& object);

	};

}