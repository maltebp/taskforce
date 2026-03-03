#pragma once

#include <string_view>
#include <functional>

#include "serialization/object-serializer.hpp"
#include "serialization/writable-list.hpp"

namespace tf {

    class WritableObject {
	public:
	
		virtual void write(std::string_view name, std::int32_t value) = 0;

		virtual void write(std::string_view name, std::int64_t value) = 0;

		virtual void write(std::string_view name, float value) = 0;

		virtual void write(std::string_view name, double value) = 0;

		virtual void write(std::string_view name, bool value) = 0;

		virtual void write(std::string_view name, std::string_view value) = 0;

		void write(std::string_view name, const std::string& value) { write(name, std::string_view{ value }); }

		template<typename TIterator>
		void write(std::string_view name, TIterator begin, TIterator end)
		{
			write_to_list(
				name,
				[&begin, &end](WritableList& list) {
					for( ; begin != end; begin++ ) {
						list.write(*begin);
					}
				}
			);
		}

		template<typename T>
		void write(std::string_view name, const T& value) {
			write_to_object(
				name,
				 [&](WritableObject& sub_object) {
					// Why the callback: the serialization implementation must be able to construct
					// a derived version of WritableObject, which is then the object written to. A
					// function that creates such a derived WritableObject would have to return it 
					// by reference/pointer, but in order to ensure it stays alive for the writing
					// scope, it would have heap allocate (or do other, more complicated tricks).
					// It is my assumption that this lambda here will not cause a heap allocation.

					ObjectSerializer<T> serializer;
					serializer.write(sub_object, value);
				}
			);
		}

	protected:

		virtual void write_to_object(std::string_view name, std::function<void(WritableObject&)> write_callback ) = 0;

		virtual void write_to_list(std::string_view name, std::function<void(WritableList&)> write_callback ) = 0;

	};

}